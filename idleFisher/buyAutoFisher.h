#pragma once

#include "math.h"

class Shader;
class text;
class animation;
struct Fcollision;

class buyAutoFisher {
public:
	buyAutoFisher(vector loc);

	void draw(Shader* shaderProgram);

	bool mouseOver();
	bool bMouseOver;

	void updateLoc();

	void calcIfPlayerInFront();
	bool inFrontPlayer = false;

private:
	bool calcMaxAutoFishers();
	void spawnAutoFisher();

	animation* plusAnim;
	text* priceText;
	double price;

	double calcPrice();

	// put in some sort of helper function class
	bool hasCurrency();

	std::vector<vector> collisionPoints = std::vector<vector>{ {13, 0}, { 26, 7 }, { 13, 14 }, { 0, 7 } };
	void setupCollision();

	Fcollision* collision;

	int calcAutoFisherId();
};