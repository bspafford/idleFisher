#pragma once

#include <SDL.h>
#include "math.h"

class AStar;
struct FpetStruct;

class Apet {
public:
	Apet(FpetStruct* pet, vector loc);
	~Apet();
	void draw(SDL_Renderer* renderer);
	void update(float deltaTime);
	void setLoc(vector loc);
	FpetStruct* getPetStruct();
private:
	AStar* Astar;

	float speed = 70;

	FpetStruct* pet;

	Fimg* img;

	vector loc;
};