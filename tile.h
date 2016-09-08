#ifndef __TILE_H_
#define __TILE_H_

#include "SDL2/SDL.h"

static const Uint8 TILE_UP = 0;
static const Uint8 TILE_DOWN = 1;
static const Uint8 TILE_FLAG = 2;
static const Uint8 TILE_QMARK = 3;

static const Uint8 GAME_BEFORE = 0;
static const Uint8 GAME_STARTED = 1;
static const Uint8 GAME_OVER = 2;
static const Uint8 GAME_WON = 3;

/** Ho deciso di creare due classi tile
    per vedere se è meglio mettere i dati in un byte o variabili separate. **/

class base_tile {
public:
	virtual void reset() = 0;
	virtual Uint8 state() = 0;
	virtual void setState(Uint8 s) = 0;
	virtual bool bomb() = 0;
	virtual void setBomb(bool b) = 0;
	virtual Uint8 number() = 0;
	virtual void setNumber(Uint8 n) = 0;
	virtual bool pressed() = 0;
	virtual void setPressed(bool p) = 0;

public:
	void cycleState();

	int getRenderSpriteNum(Uint8 gameState);
};

// Dati in un byte
class byte_tile: public base_tile {
	Uint8 data;

	// pressed:1
	// state:3
	// number/bomb:4

public:
	byte_tile() {
		reset();
	}

public:
	void reset();
	Uint8 state();
	void setState(Uint8 s);
	bool bomb();
	void setBomb(bool b);
	Uint8 number();
	void setNumber(Uint8 n);
	bool pressed();
	void setPressed(bool p);
};

// Dati con variabili separate
class full_tile: public base_tile {
	Uint8 _state;
	bool _bomb;
	bool _pressed;
	Uint8 _number;

public:
	full_tile() {
		reset();
	}

public:
	void reset();
	Uint8 state();
	void setState(Uint8 s);
	bool bomb();
	void setBomb(bool b);
	Uint8 number();
	void setNumber(Uint8 n);
	bool pressed();
	void setPressed(bool p);
};

#endif // __TILE_H_
