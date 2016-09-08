#include "loadres.h"

#include "globalvars.h"
#include "resources.h"

#include <windows.h>

SDL_Surface *icon32;
SDL_Texture *tileTex;
SDL_Texture *digitTex;
SDL_Texture *stateIconTex;

SDL_Surface *loadBMPFromResources(int RES_ID) {
	HMODULE hModule = GetModuleHandle(NULL);

	HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(RES_ID), "BMP");
	unsigned int res_size = SizeofResource(hModule, hRes);

	HGLOBAL hgRes = LoadResource(hModule, hRes);
	unsigned char* res_data = (unsigned char*)LockResource(hgRes);

	return SDL_LoadBMP_RW(SDL_RWFromConstMem(res_data, res_size), 1);
}

bool loadIcon() {
	icon32 = loadBMPFromResources(IDB_ICON32);
	if (icon32 == NULL)
		return false;

	SDL_SetColorKey(icon32, SDL_TRUE, SDL_MapRGB(icon32->format, 0xff, 0x0, 0xff));
	return true;
}

bool loadResources(SDL_Renderer *renderer) {
	SDL_Surface *tileSurface = loadBMPFromResources(IDB_TILES);
	if (tileSurface == NULL)
		return false;

	SDL_Surface *digitSurface = loadBMPFromResources(IDB_DIGITS);
	if (digitSurface == NULL)
		return false;

	SDL_SetColorKey(digitSurface, SDL_TRUE, SDL_MapRGB(digitSurface->format, 0xff, 0x0, 0xff));

	SDL_Surface *stateIconSurface = loadBMPFromResources(IDB_STATEICONS);
	if (stateIconSurface == NULL)
		return false;

	SDL_SetColorKey(stateIconSurface, SDL_TRUE, SDL_MapRGB(stateIconSurface->format, 0xff, 0x0, 0xff));

	tileTex = SDL_CreateTextureFromSurface(renderer, tileSurface);
	if (tileTex == NULL)
		return false;

	digitTex = SDL_CreateTextureFromSurface(renderer, digitSurface);
	if (digitTex == NULL)
		return false;

	stateIconTex = SDL_CreateTextureFromSurface(renderer, stateIconSurface);
	if (stateIconTex == NULL)
		return false;

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
