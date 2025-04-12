#pragma once

#include <SDL.h>

#include "math.h"

class Ubutton;

class Avault {
public:
	Avault(vector loc);
	void draw(SDL_Renderer* renderer);
	void setLoc(vector loc);

private:
	void click();
	void enterVault();

	vector loc;

	Ubutton* vaultButton;
};
