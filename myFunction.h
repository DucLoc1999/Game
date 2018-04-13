#ifndef MY_FUNCTION
#define MY_FUNCTION

#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "object.h"


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640
//
extern SDL_Window *window ;
extern SDL_Renderer *render ;
//image
extern SDL_Texture *YourTank_Texture;
extern SDL_Texture *Enemy_Texture;
extern SDL_Texture *Bullet_Texture;


SDL_Texture* LoadTexture(string imgPath);//,SDL_Renderer* render);


//bool CheckImpact(object A, object b);

void ClearWindow();

#endif
