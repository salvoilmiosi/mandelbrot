#ifndef __GAME_H_
#define __GAME_H_

#include "SDL2/SDL.h"

void gameStartup();

void gameCleanup();

void handleEvent(SDL_Event &event);

void renderGame(SDL_Renderer *renderer);

#endif // __GAME_H_
