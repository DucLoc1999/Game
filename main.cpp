#include "object.h"
//     SDL_SetHint///////////////////
SDL_Event event, lastEvent;

SDL_Texture *Start_Texture1 = NULL;
SDL_Texture *Start_Texture2 = NULL;

bool Init()
{
    if (SDL_Init(SDL_INIT_VIDEO ) < 0)
    {
        printf("can't init SDL_VIDEO\n");
        return false;
    }

    if(IMG_Init(IMG_INIT_PNG < 0))
    {
        printf( "can't init IMG_JNP / IMG_PNG\n");
        return false;
    }

    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) //?
        printf( "Warning: Linear texture filtering not enabled!" );


    window = SDL_CreateWindow( "_MY_GAME_", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) ;

    if (window == NULL)
    {
        printf( "can't create window\n");
        return false;
    }
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//?
    if (render == NULL)
	{
		printf("Could not create render %s", SDL_GetError());
		return false;
	}
    SDL_SetRenderDrawColor(render, 170, 170, 70, 255);  // background color
    SDL_RenderClear(render);

    if(render == NULL)
        printf("can't create render \n");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");    //lam anh khong bi mo, nhoe
    YourTank_Texture = LoadTexture("myTank.png");
    Enemy_Texture = LoadTexture("enemyTank.png");
    Bullet_Texture = LoadTexture("bullet.png");
    Start_Texture1 = LoadTexture("start1.png");
    Start_Texture2 = LoadTexture("start2.png");

    srand( time(NULL) );
    return true;
}


int main(int argc, char** argv){

    if(!Init())
        printf("can't init\n");

    bool end_game = false;

    {/// start
    SDL_Rect rect = {SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT - 120, 160, 60};
    SDL_RenderCopy(render,Start_Texture1, NULL, &rect);
    SDL_RenderPresent(render);
    bool decide = false;
    bool mousePressed = false;
    int x, y;
    while(!decide)
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
            {
                end_game = true;
                decide = true;
                break;
            }
            else if(event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
            {
                SDL_GetMouseState( &x, &y );
                if(x >= rect.x && y >= rect.y && x < rect.x+rect.w && y < rect.y+rect.h) // vi tri phim start
                {
                    SDL_RenderClear(render);
                    if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
                    {
                        printf("release\n");
                        mousePressed = false;
                        decide = true;
                    }
                    else if((event.type = SDL_MOUSEBUTTONDOWN )&& event.button.button == SDL_BUTTON_LEFT )
                    {
                        printf("click\n");
                        mousePressed = true;
                    }

                    if(mousePressed)
                        SDL_RenderCopy(render,Start_Texture2, NULL, &rect);
                    else
                        SDL_RenderCopy(render,Start_Texture1, NULL, &rect);


                    SDL_RenderPresent(render);
                }else
                {
                    printf("out\n");
                    if(mousePressed)
                    {
                        SDL_RenderCopy(render,Start_Texture1, NULL, &rect);
                        SDL_RenderPresent(render);
                        mousePressed = false;
                    }
                }
            }
        SDL_DestroyTexture(Start_Texture1);
        SDL_DestroyTexture(Start_Texture2);
    }

    // your tank
    My_Tank yourTank( (SCREEN_WIDTH-TANK_SIZE)/2, (SCREEN_HEIGHT-TANK_SIZE)/2, UP);
    // rand enemy
    vector<Enemy_Tank> enemy;
    for(int i = 0; i < NUM_OF_ENEMY; i++)
    {
        Enemy_Tank PreEnemy(yourTank.rect, enemy);
        enemy.push_back(PreEnemy);
    }


    while(!end_game)
    {
        SDL_RenderClear(render);
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
            {
                end_game = true;
                break;
            }

        yourTank.GetMove(event, enemy);

        yourTank.TankDraw();
        for(int i = 0; i < NUM_OF_ENEMY; i++)
        {
            if(enemy[i].life > 0)
                enemy[i].getMove(yourTank.rect, enemy);
            else if(enemy[i].rebornTime <= 0)
                enemy[i].Reborn(yourTank.rect, enemy);
            else
                enemy[i].rebornTime--;

             enemy[i].TankDraw();
       }

        SDL_RenderPresent(render);

        SDL_Delay(25);
        checkObj(yourTank, enemy);
        if(yourTank.life <= 0)
        {
            printf("YOUR SCORE IS %d", yourTank.score);
            end_game= true;
        }
    }

    //

    ClearWindow();
    return 0;
}
