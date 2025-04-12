#pragma once

#include <SDL.h>
#include "math.h"

struct Fcollision;

class Abuilding {
public:
	Abuilding(vector loc);
	~Abuilding();
	virtual void draw(SDL_Renderer* renderer);
	virtual void setLoc(vector loc);
	bool calcInFront();
	void setupCollision(std::vector<vector> pointsList);
protected:
	Fimg* buildingImg;
	vector loc;
	Fcollision* col;
	vector leftPoint, rightPoint;
private:
};