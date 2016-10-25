#ifndef __LOADRES_H_
#define __LOADRES_H_

#include "SDL2/SDL.h"

static const int TILE_W = 20;
static const int TILE_H = 20;
static const int DIGIT_W = 30;
static const int DIGIT_H = 50;
static const int ICON_W = 32;
static const int ICON_H = 32;
static const int OFFSET = 5;
static const int DISPLAY_DIGITS = 4;

extern SDL_Surface *icon32;

extern SDL_Texture *tileTex;

extern SDL_Texture *digitTex;

extern SDL_Texture *stateIconTex;

SDL_Surface *loadBMPFromResources(const char *RES_ID);

bool loadIcon();

bool loadResources(SDL_Renderer *renderer);

void freeResources();

#endif // __RESOURCES_H_
