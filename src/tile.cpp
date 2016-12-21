#include "tile.h"

// Viene chiamato quando si preme col tasto destro su di una casella
void base_tile::cycleState() {
	if (state() == TILE_UP) setState(TILE_FLAG);
	else if (state() == TILE_FLAG) setState(TILE_QMARK);
	else if (state() == TILE_QMARK) setState(TILE_UP);
}

// Funzione per trovare lo sprite di una casella (vedi res/tiles.bmp)
int base_tile::getRenderSpriteNum(Uint8 gameState) {
	int n = 0;

	switch (state()) {
	case TILE_UP:
	case TILE_QMARK:
		n = state()==TILE_UP ? 0 : 2; // Casella alzata o punto interrogativo
		if (pressed()) n = 6; // Casella premuta
		if (gameState == GAME_OVER) { // Se hai perso...
			if (bomb()) n = pressed() ? 15 : 3; // Bomba, con sfondo rosso se premuta.
		} else if (gameState == GAME_WON) { // Se hai vinto...
			n = bomb() ? 5 : 6 + number(); // Bomba con bandiera o il numero.
		}
		break;
	case TILE_FLAG:
		n = 1; // Casella con bandiera
		if (gameState == GAME_OVER || gameState == GAME_WON) // A fine partita
			n = bomb() ? 5 : 4; // Bandiera su bomba, o una X se sbagliata.
		break;
	case TILE_DOWN: // Casella vuota
		n = 6 + number(); // Mostra il numero
		if (bomb()) n = 3; // O la bomba
		break;
	}
	return n;
}

/** FUNZIONI DI byte_tile ******************************/

void byte_tile::reset() {
	data = 0;
}

Uint8 byte_tile::state() {
	return (data & 0x70) >> 4;
}

void byte_tile::setState(Uint8 s) {
	data &= 0x8f;
	data |= (s<<4) & 0x70;
}

bool byte_tile::bomb() {
	return (data & 0x0f) == 0x0f;
}

void byte_tile::setBomb(bool b) {
	if (b) data |= 0x0f;
	else data &= 0xf0;
}

bool byte_tile::pressed() {
	return (data & 0x80) != 0;
}

void byte_tile::setPressed(bool p) {
    if (p) data |= 0x80;
    else data &= 0x7f;
}

Uint8 byte_tile::number() {
	return bomb() ? 0 : data & 0x0f;
}

void byte_tile::setNumber(Uint8 n) {
	data &= 0xf0;
	data |= n & 0x0f;
}

/** FUNZIONI DI full_tile ******************************/

void full_tile::reset() {
	_state = 0;
	_bomb = false;
	_pressed = false;
	_number = 0;
}

Uint8 full_tile::state() {
	return _state;
}

void full_tile::setState(Uint8 s) {
	_state = s;
}

bool full_tile::bomb() {
	return _bomb;
}

void full_tile::setBomb(bool b) {
	_bomb = b;
}

bool full_tile::pressed() {
	return _pressed;
}

void full_tile::setPressed(bool p) {
	_pressed = p;
}

Uint8 full_tile::number() {
	return _number;
}

void full_tile::setNumber(Uint8 n) {
	_number = n;
}
