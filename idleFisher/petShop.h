#pragma once

#include "building.h"

class Shader;

class ApetShop : public Abuilding {
public:
	ApetShop(vector loc);
	~ApetShop();
	void draw(Shader* shaderProgram) override;
private:

};