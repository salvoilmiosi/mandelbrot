#include "timer.h"
#include "SDL2/SDL.h"

timer::timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void timer::start() {
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();
}

void timer::stop() {
    started = false;
    paused = false;
}

void timer::pause() {
    if (started && !paused) {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void timer::unpause() {
    if (paused) {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;

        pausedTicks = 0;
    }
}

int timer::get_ticks() {
    if (started) {
        if (paused) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }

    return 0;
}
