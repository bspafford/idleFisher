#pragma once

#include <SDL.h>

#include "math.h"

class timer;
class animation;

class Arain {
public:
	Arain();
	~Arain();
	void draw(SDL_Renderer* renderer);
	void start();
	void stop();
private:
	void updateRain();
	void removeAnimList();

	std::vector<std::string> animImgList;

	Fimg* rainDrop;
	timer* rainTimer;
	std::vector<animation*> animList;

	// temp
	std::vector<vector> locs;
	std::vector<float> deathLoc; // what y the raindrop hits the ground

	bool isStopped = false;
};