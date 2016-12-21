#ifndef __NUMBERDISPLAY_H_
#define __NUMBERDISPLAY_H_

#include "SDL2/SDL.h"

#include "resources.h"
#include "globalvars.h"

#include <cstring>
#include <cstdlib>

template<int digits>
class number_display {
	int number;

	char numString[digits];

public:
	number_display(int number = 0) : number(number) {}

public:
	void render(SDL_Renderer *renderer, int xo, int yo);

public:
	const number_display<digits> &operator = (const int n) { number = n; return *this; }
	void setNumber(const int n) { number = n; }
	int getNumber() { return number; }
};

template<int digits>
void number_display<digits>::render(SDL_Renderer *renderer, int xo, int yo) {
	SDL_Rect srcrect, dstrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = DIGIT_W;
	srcrect.h = DIGIT_H;

	dstrect.x = 0;
	dstrect.y = yo;
	dstrect.w = DIGIT_W;
	dstrect.h = DIGIT_H;

    snprintf(numString, digits, "%d", number);

	int z = 0;

	for (int i=digits-1; i>=0; --i) {
		char c = numString[i];
		if (c != '\0') {
			int n = c == '-' ? 10 : c - '0';

			srcrect.x = n * DIGIT_W;
			dstrect.x = (i+z) * DIGIT_W + xo;

			SDL_RenderCopy(renderer, digitTex, &srcrect, &dstrect);
		} else ++z;
	}

	while(z-->0) {
		srcrect.x = 11 * DIGIT_W;
		dstrect.x = z * DIGIT_W + xo;

		SDL_RenderCopy(renderer, digitTex, &srcrect, &dstrect);
	}
}


#endif // __NUMBERDISPLAY_H_
