#pragma once

#include "building.h"

class AmerchantShop : public Abuilding {
public:
	AmerchantShop(vector loc);
	~AmerchantShop();
	void draw(Shader* shaderProgram) override;
private:

};