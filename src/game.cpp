#include "game.h"

#include "minesweeper.h"
#include "numberdisplay.h"

#include "globalvars.h"
#include "resources.h"

#include "timer.h"

static number_display<DISPLAY_DIGITS> flag_nd;
static number_display<DISPLAY_DIGITS> time_nd;

static minesweeper *ms;

static timer game_timer;

static tile *pressed = NULL;
static int buttonDown = -1;
static bool doubleButtons = false;
static bool keyboardInput = false;

static int key_tx = 0;
static int key_ty = 0;

static int xoffset;
static int yoffset;

void gameStartup() {
	ms = new minesweeper;
}

void gameCleanup() {
	if (ms != NULL)
		delete ms;
}

static void press(int tx, int ty) {
	pressed = ms->press(tx, ty);
	if (buttonDown == SDL_BUTTON_MIDDLE) {
		ms->press(tx-1, ty-1);
		ms->press(tx  , ty-1);
		ms->press(tx+1, ty-1);
		ms->press(tx-1, ty  );
		ms->press(tx+1, ty  );
		ms->press(tx-1, ty+1);
		ms->press(tx  , ty+1);
		ms->press(tx+1, ty+1);
	}
}

static void mouseDown(int mousex, int mousey, int button) {
	if (buttonDown != -1) {
		button = SDL_BUTTON_MIDDLE;
		doubleButtons = true;
	}

	buttonDown = button;

	mousex -= xoffset;
	mousey -= yoffset;

	int x = mousex / TILE_W;
	int y = mousey / TILE_H;

	tile *t = ms->getItem(x, y);
	if (mousex < 0 || mousex >= GRID_W * TILE_W) t = NULL;
	if (mousey < 0 || mousey >= GRID_H * TILE_H) t = NULL;

	if (t == NULL) return;

	switch (ms->getState()) {
	case GAME_STARTED:
	case GAME_BEFORE:
		switch(button) {
		case SDL_BUTTON_LEFT:
		case SDL_BUTTON_MIDDLE:
			press(x, y);
			break;
		case SDL_BUTTON_RIGHT:
			t->cycleState();
		}
	}
}

static void mouseUp(int mousex, int mousey, int button) {
	if (buttonDown < 0) return;

	if (doubleButtons) {
		button = SDL_BUTTON_MIDDLE;
		doubleButtons = false;
	}

	buttonDown = -1;
	pressed = NULL;

	mousex -= xoffset;
	mousey -= yoffset;

	int x = mousex / TILE_W;
	int y = mousey / TILE_H;

	tile *t = ms->getItem(x, y);
	if (mousex < 0 || mousex >= GRID_W * TILE_W) t = NULL;
	if (mousey < 0 || mousey >= GRID_H * TILE_H) t = NULL;

	switch (ms->getState()) {
	case GAME_BEFORE:
		if (t != NULL && button == SDL_BUTTON_LEFT) {
			if (t->state() == TILE_UP || t->state() == TILE_QMARK) {
				ms->addBombs(x, y);
				pressed = ms->press(x, y);
				game_timer.start();
				// fall through
			} else {
				break;
			}
		} else {
			ms->unpressAll();
			break;
		}
	if (ms->getState() != GAME_STARTED) break;
	// else fall through
	case GAME_STARTED:
		if (t != NULL) {
			if (button == SDL_BUTTON_LEFT) {
				ms->clickPressed();
			} else if (button == SDL_BUTTON_MIDDLE) {
				if (ms->isValidMiddleClick(x, y)) {
					ms->clickPressed();
				}
			}
		}
		if (ms->isGameOver())
			game_timer.pause();
		else ms->unpressAll();
	break;
	case GAME_OVER:
	case GAME_WON:
		if (button == SDL_BUTTON_LEFT) {
			ms->reset();
			pressed = NULL;
			game_timer.stop();
		}
	}
}

static void mouseMoved(int mousex, int mousey) {
	mousex -= xoffset;
	mousey -= yoffset;

	int x = mousex / TILE_W;
	int y = mousey / TILE_H;

	tile *t = ms->getItem(x, y);
	if (mousex < 0 || mousex >= GRID_W * TILE_W) t = NULL;
	if (mousey < 0 || mousey >= GRID_H * TILE_H) t = NULL;

	switch (ms->getState()) {
	case GAME_BEFORE:
	case GAME_STARTED:
		if (buttonDown == SDL_BUTTON_LEFT || buttonDown == SDL_BUTTON_MIDDLE) {
			if (t != pressed) {
				ms->unpressAll();
				pressed = t;
				if (t != NULL) press(x, y);
			}
		}
	}
}

static void keyDown(SDL_Keycode key) {
	switch (key) {
	case SDLK_LEFT:
		if (--key_tx < 0) key_tx += GRID_W;
		mouseMoved(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset);
		break;
	case SDLK_RIGHT:
		if (++key_tx >= GRID_W) key_tx -= GRID_W;
		mouseMoved(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset);
		break;
	case SDLK_UP:
		if (--key_ty < 0) key_ty += GRID_H;
		mouseMoved(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset);
		break;
	case SDLK_DOWN:
		if (++key_ty >= GRID_H) key_ty -= GRID_H;
		mouseMoved(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset);
		break;
	case SDLK_SPACE:
		mouseDown(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_LEFT);
		break;
	case SDLK_z:
		mouseDown(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_RIGHT);
		break;
	case SDLK_x:
		mouseDown(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_MIDDLE);
		break;
	default:
		break;
	}
}

static void keyUp(SDL_Keycode key) {
	switch (key) {
	case SDLK_SPACE:
		mouseUp(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_LEFT);
		break;
	case SDLK_z:
		mouseUp(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_RIGHT);
		break;
	case SDLK_x:
		mouseUp(key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, SDL_BUTTON_MIDDLE);
		break;
	default:
		break;
	}
}

void handleEvent(SDL_Event& event) {
	switch(event.type) {
	case SDL_MOUSEBUTTONDOWN:
		keyboardInput = false;
		mouseDown(event.button.x, event.button.y, event.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		keyboardInput = false;
		mouseUp(event.button.x, event.button.y, event.button.button);
		break;
	case SDL_MOUSEMOTION:
		if (!keyboardInput) {
			mouseMoved(event.motion.x, event.motion.y);
		}
		break;
	case SDL_KEYDOWN:
		keyboardInput = true;
		keyDown(event.key.keysym.sym);
		break;
	case SDL_KEYUP:
		keyboardInput = true;
		keyUp(event.key.keysym.sym);
		break;
	}
}

void renderStateIcon(SDL_Renderer *renderer, int xo, int yo) {
	int n = 0;

	switch (ms->getState()) {
	case GAME_OVER:
		n = 0;
		break;
	case GAME_WON:
		n = 1;
		break;
	default:
		n = (buttonDown == SDL_BUTTON_LEFT || buttonDown == SDL_BUTTON_MIDDLE) ? 3 : 2;
	}

	SDL_Rect srcrect = {n * ICON_W, 0, ICON_W, ICON_H};

	SDL_Rect dstrect = {xo, yo, ICON_W, ICON_H};

	SDL_RenderCopy(renderer, stateIconTex, &srcrect, &dstrect);
}

void renderGame(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0x90,0x90,0x90,0xff);
	SDL_RenderClear(renderer);

	if (ms->getState() == GAME_BEFORE) {
		flag_nd = MINE_NUM;
	} else {
		flag_nd = ms->countAllBombs() - ms->countAllFlags();
	}
	flag_nd.render(renderer, OFFSET, OFFSET);

	int min_w = DIGIT_W*DISPLAY_DIGITS*2 + 4*OFFSET + ICON_W;
	int screen_w = TILE_W * GRID_W + 2 * OFFSET;
	if (screen_w < min_w) screen_w = min_w;

	int timex = screen_w - DIGIT_W * DISPLAY_DIGITS - OFFSET;
	int gridx = (screen_w - GRID_W*TILE_W) / 2;
	int gridy = DIGIT_H + 2 * OFFSET;
	int iconx = (screen_w - ICON_W) / 2;
	int icony = (DIGIT_H - ICON_H) / 2 + OFFSET;

	time_nd = game_timer.get_ticks() / 1000;
	time_nd.render(renderer, timex, OFFSET);

	ms->render(renderer, xoffset = gridx, yoffset = gridy);

	if (keyboardInput) {
		SDL_Rect rect = {key_tx * TILE_W + xoffset, key_ty * TILE_H + yoffset, TILE_W, TILE_H};
		SDL_SetRenderDrawColor(renderer, 0xff,0x0,0x0,0xff);
		SDL_RenderDrawRect(renderer, &rect);
	}

	renderStateIcon(renderer, iconx, icony);

	SDL_RenderPresent(renderer);
}
