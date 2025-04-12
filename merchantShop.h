#pragma once

#include "building.h"

class AmerchantShop : public Abuilding {
public:
	AmerchantShop(vector loc);
	~AmerchantShop();
	void draw(SDL_Renderer* renderer) override;
private:

};