#pragma once

#include "building.h"

class ApetShop : public Abuilding {
public:
	ApetShop(vector loc);
	~ApetShop();
	void draw(SDL_Renderer* renderer) override;
private:

};