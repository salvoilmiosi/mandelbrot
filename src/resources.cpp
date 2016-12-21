#include "resources.h"

#include "globalvars.h"

#include "../resource_pack/include/resource_load.h"

SDL_Surface *icon32;
SDL_Texture *tileTex;
SDL_Texture *digitTex;
SDL_Texture *stateIconTex;

SDL_Surface *loadBMPFromResources(const char *RES_ID) {
	SDL_RWops *rw = getResourceRW(RES_ID);
	if (rw)	return SDL_LoadBMP_RW(rw, 1);
	else return nullptr;
}

bool loadResources(const std::string &filename, SDL_Renderer *renderer) {
	if (!openResourceFile(filename.c_str()))
		return false;

	if (nullptr == (icon32 = loadBMPFromResources("IDB_ICON32"))) return false;

	SDL_SetColorKey(icon32, SDL_TRUE, SDL_MapRGB(icon32->format, 0xff, 0x0, 0xff));

	SDL_Surface *tileSurface = loadBMPFromResources("IDB_TILES");
	if (!tileSurface) return false;

	SDL_Surface *digitSurface = loadBMPFromResources("IDB_DIGITS");
	if (!digitSurface) return false;

	SDL_SetColorKey(digitSurface, SDL_TRUE, SDL_MapRGB(digitSurface->format, 0xff, 0x0, 0xff));

	SDL_Surface *stateIconSurface = loadBMPFromResources("IDB_STATEICONS");
	if (!stateIconSurface) return false;

	SDL_SetColorKey(stateIconSurface, SDL_TRUE, SDL_MapRGB(stateIconSurface->format, 0xff, 0x0, 0xff));

	if (nullptr == (tileTex = SDL_CreateTextureFromSurface(renderer, tileSurface))) return false;
	if (nullptr == (digitTex = SDL_CreateTextureFromSurface(renderer, digitSurface))) return false;
	if (nullptr == (stateIconTex = SDL_CreateTextureFromSurface(renderer, stateIconSurface))) return false;

	SDL_FreeSurface(tileSurface);
	SDL_FreeSurface(digitSurface);
	SDL_FreeSurface(stateIconSurface);
	return true;
}

void freeResources() {
	SDL_FreeSurface(icon32);

	SDL_DestroyTexture(tileTex);
	SDL_DestroyTexture(digitTex);
	SDL_DestroyTexture(stateIconTex);
}
