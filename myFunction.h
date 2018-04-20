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
#define SCREEN_HEIGHT 672
//
extern SDL_Window *window ;
extern SDL_Renderer *render ;
//image
extern SDL_Texture *YourTank_Texture;
extern SDL_Texture *Enemy_Texture;
extern SDL_Texture *Enemy_Texture1;
extern SDL_Texture *Enemy_Texture2;
extern SDL_Texture *Enemy_Texture3;
extern SDL_Texture *Bullet_Texture;
extern SDL_Texture *Heart_Texture;
extern SDL_Texture *StatusBar_Texture;
extern SDL_Texture *Pause_Texture;
extern SDL_Texture *Play_Texture;
//text
extern TTF_Font *font;
//event
extern SDL_Event event;


SDL_Texture* LoadTexture(const string& imgPath);//,SDL_Renderer* render);

//void WriteText();

bool MouseIn(const SDL_Rect& rect);

void DrawStartusBar(const int& life, const int& score);

void ClearWindow();

bool Start();

int GameRun();

bool EndGame(const int& score);

#endif
