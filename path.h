#pragma once

#include "math.h"
#include "line.h"

class Path {
public:
	std::vector<vector> lookPoints;
	std::vector<Line*> turnBoundaries;
	int finishLineIndex;

	Path(std::vector<vector> waypoints, vector startPos, float turnDst);
	void draw(SDL_Renderer* renderer);
};