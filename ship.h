#pragma once

#include <SDL.h>
#include "math.h"

class timer;

class Aship {
public:
	Aship(vector loc);
	~Aship();
	void draw(SDL_Renderer* renderer);
private:
	void shipbob();
	timer* bobTimer;
	Fimg* shipImg;

	vector loc;
	vector tempLoc;
};