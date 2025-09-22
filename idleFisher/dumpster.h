#pragma once

#include "math.h"

class Shader;
class Image;

class dumpster {
public:
	dumpster(vector loc);

	void onHover();
	void sellFish();
	void draw(Shader* shaderProgram);
	
private:
	bool mouseOver();

	bool bMouseOver = false;


	Image* img;

	vector loc;

};