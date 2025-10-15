#pragma once

#include "building.h"

class Shader;
struct Fcollision;

class AmechanicHouse : public Abuilding {
public:
	AmechanicHouse(vector loc);
	~AmechanicHouse();
	void draw(Shader* shaderProgram) override;
};