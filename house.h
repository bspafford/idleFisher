#pragma once

#include "building.h"

class Ubutton;

class Ahouse : public Abuilding {
public:
	Ahouse(vector loc);
	~Ahouse();
	void draw(SDL_Renderer* renderer) override;
	void setLoc(vector loc) override;
private:
	void enterVault();

	Ubutton* door;
};