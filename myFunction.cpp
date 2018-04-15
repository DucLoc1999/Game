#include "myFunction.h"


//
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
//image
SDL_Texture *YourTank_Texture = NULL;
SDL_Texture *Enemy_Texture = NULL;
SDL_Texture *Bullet_Texture = NULL;
//text
TTF_Font *font = NULL;
//event
SDL_Event event;

SDL_Texture* LoadTexture(string imgPath)//, SDL_Renderer* render)
{
    SDL_Surface* img = IMG_Load(imgPath.c_str());

    SDL_SetColorKey(img, SDL_TRUE , SDL_MapRGB(img->format, 255, 255, 255));   //xử lý colo key

    if(img == NULL)
        cout << "not found " << imgPath << endl;


    SDL_Texture* newTexture = NULL;

    newTexture = SDL_CreateTextureFromSurface(render,img);
    SDL_FreeSurface(img);


    return newTexture;
}

void ClearWindow()
{
    SDL_DestroyTexture(YourTank_Texture);
    YourTank_Texture = NULL;
    SDL_DestroyTexture(Enemy_Texture);
    Enemy_Texture = NULL;
    SDL_DestroyTexture(Bullet_Texture);
    Bullet_Texture = NULL;

    //windown
    SDL_DestroyRenderer(render );
    SDL_DestroyWindow(window );
    window = NULL;
    render = NULL;

    //font
    TTF_CloseFont(font);

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

bool Start()
{
    SDL_Texture *Start_Texture1 = LoadTexture("start1.png");
    SDL_Texture *Start_Texture2 = LoadTexture("start2.png");

    SDL_RenderClear(render);
    SDL_Rect rect = {SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT - 120, 160, 60};
    SDL_RenderCopy(render, Start_Texture1, NULL, &rect);
    SDL_RenderPresent(render);
    bool decide = false;
    bool mousePressed = false;
    bool play = true;
    int x, y;

    while(!decide)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
            {
                play = false;
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
                        play = true;
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
    }

    SDL_DestroyTexture(Start_Texture1);
    SDL_DestroyTexture(Start_Texture2);
    return play;
}

int GameRun()
{
        bool end_game = !true;
        // your tank
        My_Tank yourTank( (SCREEN_WIDTH-TANK_SIZE)/2, (SCREEN_HEIGHT-TANK_SIZE)/2, UP);
        // enemy
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

    return yourTank.score;
}

bool EndGame(int score)
{
    SDL_Rect rect;

    {///shown score
    SDL_Color textColor;
    SDL_RenderClear(render);
    SDL_Texture* textTexture = NULL;
    SDL_Surface* textSurface = NULL;

    stringstream SCORE;
    SCORE.str( "" );
    if(score < 100)
        SCORE << "  ";
    if(score < 10)
        SCORE << "  ";
    SCORE << score;
    if(score < 100)
        SCORE << "  ";
    if(score < 10)
        SCORE << "  ";

    textColor = {255, 255, 0, 255};//vang
    textSurface = TTF_RenderText_Solid(font, "YOUR SCORE IS", textColor );
    textTexture = SDL_CreateTextureFromSurface(render, textSurface );
    rect = {SCREEN_WIDTH/2 - 350, SCREEN_HEIGHT/2 - 80 - 100, 700, 160};
    SDL_RenderCopyEx( render, textTexture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

    if(score < 10)
        textColor = {255, 255, 255, 255};
    else
        textColor = {0, 0, 255, 255};//lam
    if(score >= 50)
        textColor = {128, 0, 128, 255};//tim
    if(score >= 100)
        textColor = {255, 128, 0, 255};//cam
    textSurface = TTF_RenderText_Solid(font, SCORE.str().c_str(), textColor );
    textTexture = SDL_CreateTextureFromSurface(render, textSurface);
    rect = {SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 100 + 80, 400, 200};
    SDL_RenderCopyEx( render, textTexture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

    textColor = {255, 0, 0, 255};//do
    textSurface = TTF_RenderText_Solid(font, "Click here to play again", textColor );
    textTexture = SDL_CreateTextureFromSurface(render, textSurface );
    rect = {SCREEN_WIDTH/2 - 300, SCREEN_HEIGHT/2 - 25 + 250, 600, 50};
    SDL_RenderCopyEx( render, textTexture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    //Update screen
    SDL_RenderPresent( render );
    }

    //play again or quit
    bool playAgain = false;
    while(!playAgain)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                return true;
            else
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if(x >= rect.x && y >= rect.y && x < rect.x+rect.w && y < rect.y+rect.h)
                    if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
                        playAgain = true;
            }
    }
    printf("play again");
}


