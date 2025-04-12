#pragma once

#include <SDL.h>
#include "math.h"

class animation;
class timer;
struct Fimg;

class AfishSchool {
public:
	AfishSchool(vector loc);
	~AfishSchool();
	void draw(SDL_Renderer* renderer);
	void setLoc(vector loc);
	AfishSchool* pointInSchool(vector worldPoint);
	void removeFishNum();
private:
	void remove();

	vector loc;

	animation* anim;

	// amount of times you can fish from, random
	// alive time, random
	timer* lifeTimer;
	float fishNum;
};
