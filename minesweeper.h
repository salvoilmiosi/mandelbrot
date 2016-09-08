#ifndef __MINESWEEPER_H_
#define __MINESWEEPER_H_

#include "SDL2/SDL.h"

#include "grid.h"
#include "tile.h"

// Qua decido che tipo di casella usare
typedef byte_tile tile;
typedef grid<tile> tile_grid;

class minesweeper: public tile_grid {
	Uint8 gameState;

public:
	minesweeper();

	tile *getItem(int x, int y);

public:
	void reset();

	void addBombs(int px, int py);

	bool isValidMiddleClick(int x, int y);

	int countAllFlags();

	int countAllBombs();

	tile *press(int x, int y);

	void unpressAll();

	void clickPressed();

	bool isGameOver() {
		return gameState == GAME_OVER || gameState == GAME_WON;
	}

public:
	Uint8 getState() { return gameState; }

public:
	void render(SDL_Renderer *renderer, int xo, int yo);

private:
	void toggleTiles(int x, int y);

	void checkVictory();
};

#endif // __MINESWEEPER_H_
