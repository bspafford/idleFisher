#pragma once

#include "math.h"
#include "line.h"

class Shader;

class Path {
public:
	std::vector<vector> lookPoints;
	std::vector<Line*> turnBoundaries;
	int finishLineIndex;

	Path(std::vector<vector> waypoints, vector startPos, float turnDst);
	~Path();
	void draw(Shader* shaderProgram);
};