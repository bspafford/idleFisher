#pragma once

#include <SDL.h>

#include "math.h"

struct FfishData;
struct FsaveFishData;
struct FcurrencyStruct;

class UfishNumWidget {
public:
	~UfishNumWidget();

	// one for fish, and the other for currency
	void setup(SDL_Renderer* renderer, FfishData* fish, FsaveFishData* saveFish, int fishQuality);
	void setup(SDL_Renderer* renderer, FcurrencyStruct* currency, double num);

	void setupText(SDL_Renderer* renderer);

	void draw(SDL_Renderer* renderer, float x, float y1);

	SDL_Texture* text;
	SDL_Surface* surfaceText;
	int textW, textH;

	SDL_Texture* texture;
	int w, h;
	vector widgetSize;
	int y = 0;

	Fimg* star;

	FfishData* fish;
	FsaveFishData* saveFish;
	int fishQuality;
	double currencyNum;
};