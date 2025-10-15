#pragma once

#include "building.h"

class Shader;
class Ubutton;

class Ahouse : public Abuilding {
public:
	Ahouse(vector loc);
	~Ahouse();
	void draw(Shader* shaderProgram) override;
	void setLoc(vector loc) override;
private:
	void enterVault();

	std::unique_ptr<Ubutton> door;
};