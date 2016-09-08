#include "timer_thread.h"

#include "SDL2/SDL.h"

static bool running = false;

static SDL_Thread *thread;

static int timer_thread_func(void *data) {
	timer *t = (timer *)data;

	static int eventType = SDL_RegisterEvents(1);

	static int lastTicks = 0;
	static int ticks = 0;

	running = true;
	while (running) {
		if (t->is_started()) {
			ticks = t->get_ticks() % 1000;

			if (lastTicks==0) lastTicks = ticks;

			if (ticks < lastTicks) {
				SDL_Event e;
				e.type = eventType;
				SDL_PushEvent(&e);
			}

			lastTicks = ticks;
		}

		SDL_Delay(10);
	}

	return 0;
}

void timer_thread_start(timer *t) {
	thread = SDL_CreateThread(timer_thread_func, "Timer thread", t);
}

void timer_thread_stop() {
	if (running) {
		running = false;
		SDL_WaitThread(thread, NULL);
	}
}
