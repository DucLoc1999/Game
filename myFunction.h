#ifndef MY_FUNCTION
#define MY_FUNCTION

#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <sstream>

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
//text
extern TTF_Font *font;
//event
extern SDL_Event event;

SDL_Texture* LoadTexture(string imgPath);//,SDL_Renderer* render);

void ClearWindow();

bool Start();

int GameRun();

bool EndGame(int score);

#endif
