#pragma once

#include "building.h"

struct Fcollision;

class AmechanicHouse : public Abuilding {
public:
	AmechanicHouse(vector loc);
	~AmechanicHouse();
	void draw(SDL_Renderer* renderer) override;
private:
	Fcollision* col;
};