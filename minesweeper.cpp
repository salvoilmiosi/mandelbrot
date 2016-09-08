#include "minesweeper.h"

#include "globalvars.h"
#include "loadres.h"

#include <cstring>
#include <cstdlib>

minesweeper::minesweeper() : tile_grid(GRID_W, GRID_H) {
	gameState = GAME_BEFORE;
}

// Overload di getItem - aggiunge la funzionalità
// di avere una griglia "infinita"
tile *minesweeper::getItem(int x, int y) {
	if (WRAP_AROUND) {
		while (x<0) x+=w;
		while (x>=w) x-=w;
		while (y<0) y+=h;
		while (y>=h) y-=h;

        return data + (y*w + x);
	} else {
		return grid::getItem(x, y);
	}
}

void minesweeper::reset() {
	for (tile *t=data; t-data<datasize; ++t) {
		t->reset();
	}

	gameState = GAME_BEFORE;
}

// Macro per controllare caselle in un quadrato 3x3 centrato in x,y
#define IF_EXISTS(ti, x, y) if((ti = getItem(x, y)))
#define CHECK_ADJ(ti, x, y, expr) \
IF_EXISTS(ti, x-1, y-1) expr;\
IF_EXISTS(ti, x,   y-1) expr;\
IF_EXISTS(ti, x+1, y-1) expr;\
IF_EXISTS(ti, x-1, y  ) expr;\
IF_EXISTS(ti, x  , y  ) expr;\
IF_EXISTS(ti, x+1, y  ) expr;\
IF_EXISTS(ti, x-1, y+1) expr;\
IF_EXISTS(ti, x  , y+1) expr;\
IF_EXISTS(ti, x+1, y+1) expr;

// Aggiunge le bombe casualmente nella griglia
void minesweeper::addBombs(int px, int py) {
	srand(SDL_GetTicks());

	int x, y;
	tile *t;

	// Creo un array di bool che indica i "posti liberi", le caselle senza bombe
	bool freeSpots[datasize];
	memset(freeSpots, -1, datasize);
	int spotCount = datasize;

	// Questo controlla che non appaiano bombe vicino
	// a dove è stato fatto il primo click.
	CHECK_ADJ(t, px, py, {freeSpots[t-data]=false; --spotCount;});

	int mn = MINE_NUM; // Numero di bombe da mettere
	int spot; // Posto libero casuale in freeSpots
	int i; // Indice libero nella griglia

	// O finiscono le bombe da mettere o i posti liberi
	while (spotCount>0 && mn>0) {
        spot = rand() % spotCount;
        // Itero in freeSpots per trovare il posto libero n. spot
        for (i=0; i<datasize; ++i) {
			if (freeSpots[i] && --spot < 0) {
				data[i].setBomb(true);
				freeSpots[i] = false;
				--spotCount;
				--mn;
				break;
			}
        }
	}

	// Conta le bombe adiacenti alle celle
	for (int i=0; i<datasize; ++i) {
		if (data[i].bomb()) continue;

		x = i % GRID_W;
		y = i / GRID_W;

		int n = 0;
		CHECK_ADJ(t, x, y, if(t->bomb()) ++n);

		data[i].setNumber(n);
	}

	gameState = GAME_STARTED;
}

// Posso cliccare col tasto centrale solo nelle caselle con un numero
// e solo se attorno a essa ci siano lo stesso numero di bandiere.
bool minesweeper::isValidMiddleClick(int x, int y) {
	tile *t = getItem(x, y);
	if (t->state() != TILE_DOWN) return false;
	int n = t->number();
	CHECK_ADJ(t, x, y, if (t->state() == TILE_FLAG) --n);
	return n == 0;
}

#undef IF_EXISTS
#undef CHECK_ADJ

// Conto il numero di caselle con le bandiere.
int minesweeper::countAllFlags() {
	int n = 0;
	for (tile *t=data; t-data<datasize; ++t) {
		if (t->state() == TILE_FLAG) ++n;
	}
	return n;
}

// Conto il numero di caselle con le bombe.
int minesweeper::countAllBombs() {
	int n = 0;
	for (tile *t=data; t-data<datasize; ++t) {
		if (t->bomb()) ++n;
	}
	return n;
}

// Imposta una casella come "premuta"
tile *minesweeper::press(int x, int y) {
	tile *t = getItem(x, y);
	if (t == NULL) return NULL;

	t->setPressed(true);
	return t;
}

// Rilascia tutte le caselle
void minesweeper::unpressAll() {
	for (tile *t=data; t-data<datasize; ++t) {
		t->setPressed(false);
	}
}

// Funzione ricorsiva per cliccare a espansione le caselle vuote.
void minesweeper::toggleTiles(int x, int y) {
	tile *t = getItem(x, y);
	if (t == NULL) return;

	if (t->state() == TILE_DOWN) return;
	if (t->state() == TILE_FLAG) return;
	if (t->bomb()) return; // Non dovrebbe succedere

	t->setState(TILE_DOWN);
	if (t->number() > 0) return;

	toggleTiles(x-1, y-1);
	toggleTiles(x, y-1);
	toggleTiles(x+1, y-1);
	toggleTiles(x-1, y);
	toggleTiles(x+1, y);
	toggleTiles(x-1, y+1);
	toggleTiles(x, y+1);
	toggleTiles(x+1, y+1);
}

// Gestisce l'azione al clic in tutte le caselle premute
void minesweeper::clickPressed() {
	int x, y;
	tile *t;
	for (int i=0; i<datasize; ++i) {
		t = data + i;
		if (t->pressed()) {
			x = i % GRID_W;
			y = i / GRID_W;
			if (t->state() == TILE_UP || t->state() == TILE_QMARK) {
				if (t->bomb()) gameState = GAME_OVER;
				else toggleTiles(x, y);
			}
			// Non rilascio le caselle bomba così nella schermata di game over
			// si vedano segnate le bombe cliccate.
			if (!t->bomb())
				t->setPressed(false);
		}
	}

	if (gameState != GAME_OVER)
		checkVictory();
}

// Controlla la vittoria:
// se tutte le caselle alzate hanno la bomba allora si ha vinto.
void minesweeper::checkVictory() {
	int ups = 0;
	for (tile *t=data; t-data<datasize; ++t) {
		if (t->state() == TILE_UP || t->state() == TILE_FLAG) ++ups;
	}
	if (ups == countAllBombs()) gameState = GAME_WON;
}

// Funzione per renderizzare la griglia
void minesweeper::render(SDL_Renderer *renderer, int xo, int yo) {
	SDL_Rect srcrect, dstrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = TILE_W;
	srcrect.h = TILE_H;

	dstrect = srcrect;

	int x, y;

	for (int i=0; i<datasize; ++i) {
		tile *t = data + i;

		x = i % GRID_W;
		y = i / GRID_W;

		srcrect.x = t->getRenderSpriteNum(gameState) * TILE_W;

		dstrect.x = x * TILE_W + xo;
		dstrect.y = y * TILE_H + yo;

		SDL_RenderCopy(renderer, tileTex, &srcrect, &dstrect);
	}
}
