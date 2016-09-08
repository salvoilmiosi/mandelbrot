#ifndef __NUMBERDISPLAY_H_
#define __NUMBERDISPLAY_H_

#include "SDL2/SDL.h"

class number_display {
	int number;
	const int digits;

	char *numString;

public:
	number_display(int digits);

	virtual ~number_display();

public:
	void render(SDL_Renderer *renderer, int xo, int yo);

public:
	void setNumber(const int n) { number = n; }
	int getNumber() { return number; }
};

#endif // __NUMBERDISPLAY_H_
