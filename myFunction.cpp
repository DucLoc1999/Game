#include "myFunction.h"


//
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
//image
SDL_Texture *YourTank_Texture = NULL;
SDL_Texture *Enemy_Texture = NULL;
SDL_Texture *Bullet_Texture = NULL;

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
//surface
SDL_DestroyTexture(YourTank_Texture);
YourTank_Texture = NULL;
SDL_DestroyTexture(Enemy_Texture);
Enemy_Texture = NULL;
SDL_DestroyTexture(Bullet_Texture);
Bullet_Texture = NULL;
//object

//windown
SDL_DestroyRenderer(render );
SDL_DestroyWindow(window );
window = NULL;
render = NULL;

//Quit SDL subsystems
IMG_Quit();
SDL_Quit();
}

