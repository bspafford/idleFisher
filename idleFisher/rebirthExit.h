#pragma once

#include "math.h"

class Shader;
class Ubutton;

class ArebirthExit {
public:
	ArebirthExit(vector loc);

	void draw(Shader* shaderProgram);

private:
	void exitWorld();

	Ubutton* door;
};