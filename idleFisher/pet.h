#pragma once

#include "math.h"

class AStar;
class Shader;
struct FpetStruct;

class Apet {
public:
	Apet(FpetStruct* pet, vector loc);
	~Apet();
	void draw(Shader* shaderProgram);
	void update(float deltaTime);
	void setLoc(vector loc);
	FpetStruct* getPetStruct();
private:
	AStar* Astar;

	float speed = 70;

	FpetStruct* pet;

	Image* img;

	vector loc;
};