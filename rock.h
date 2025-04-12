#pragma once

#include "building.h"

class Arock : public Abuilding {
public:
	Arock(vector loc);
	~Arock();
	void draw(SDL_Renderer* renderer) override;
private:
	Fcollision* col;
};