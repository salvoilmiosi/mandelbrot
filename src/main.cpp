#include "SDL2/SDL.h"

#include "resources.h"
#include "globalvars.h"

#include "game.h"
#include "timer.h"

#include <string>

static SDL_Window *window;
static SDL_Renderer *renderer;

static const int FPS = 100;

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return 1;

	std::string path;
	if (argc > 0) {
		path = argv[0];
		path = path.substr(0, 1 + path.find_last_of("\\/"));
	}

    readOptions(path + "options.txt");

	int min_w = DIGIT_W*DISPLAY_DIGITS*2 + 4*OFFSET + ICON_W;
	int w = TILE_W * GRID_W + 2 * OFFSET;
	if (w < min_w) w = min_w;
	int h = TILE_H * GRID_H + DIGIT_H + 3 * OFFSET;

	window = SDL_CreateWindow("Minesweeper",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w, h, SDL_WINDOW_SHOWN);
	if (window == NULL)
		return 2;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		return 2;

	if (!loadResources(path + "resource.res", renderer))
		return 2;
	
	SDL_SetWindowIcon(window, icon32);

	gameStartup();

	timer fps;

	bool quit = false;
	SDL_Event event;
    while (!quit) {
		fps.start();

		renderGame(renderer);

        while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				// fall through
			default:
				handleEvent(event);
				break;
			}
        }

		if (fps.get_ticks() < 1000 / FPS) {
            SDL_Delay(1000 / FPS - fps.get_ticks());
        }
    }

    gameCleanup();
    saveOptions(path + "options.txt");

	freeResources();
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
    return 0;
}
