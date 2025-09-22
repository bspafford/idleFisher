#pragma once

#include "math.h"

class Shader;
class timer;

class Aship {
public:
	Aship(vector loc);
	~Aship();
	void draw(Shader* shaderProgram);
private:
	void shipbob();
	timer* bobTimer;
	Image* shipImg;

	vector loc;
	vector tempLoc;
};