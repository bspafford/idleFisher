#pragma once

#include "math.h"

class dumpster {
public:
	dumpster(vector loc);

	void onHover();
	void sellFish();
	void draw(SDL_Renderer* renderer);
	
private:
	bool mouseOver();

	bool bMouseOver = false;


	Fimg* img;

	vector loc;

};