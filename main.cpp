#include "object.h"

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

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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

    srand( time(NULL) );

    font = TTF_OpenFont( "ALIEES__.TTF", 35 );
	if( font == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		return false;
	}


    return true;
}


int main(int argc, char** argv){

    if(!Init())
        printf("can't init\n");

    while(Start())
    {
        int score = GameRun();
        if(EndGame(score))
            break;
    }

    ClearWindow();

    //

    return 0;
}
