#include "numberdisplay.h"

#include "loadres.h"
#include "globalvars.h"

#include <cstring>
#include <cstdlib>

number_display::number_display(int digits) : digits(digits) {
	number = 0;

	numString = new char[digits];
}

number_display::~number_display() {
	delete []numString;
}

void number_display::render(SDL_Renderer *renderer, int xo, int yo) {
	SDL_Rect srcrect, dstrect;

	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = DIGIT_W;
	srcrect.h = DIGIT_H;

	dstrect.x = 0;
	dstrect.y = yo;
	dstrect.w = DIGIT_W;
	dstrect.h = DIGIT_H;

	memset(numString, '\0', digits);

	itoa(number, numString, 10);

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
