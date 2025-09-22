#pragma once

#include "math.h"

class Shader;
class animation;
class timer;
struct Image;

class AfishSchool {
public:
	AfishSchool(vector loc);
	~AfishSchool();
	void draw(Shader* shaderProgram);
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
