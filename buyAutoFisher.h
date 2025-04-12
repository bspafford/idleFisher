#pragma once

#include "math.h"

class text;
class animation;
struct Fcollision;


class buyAutoFisher {
public:
	buyAutoFisher(vector loc);

	void draw(SDL_Renderer* renderer);

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

	SDL_Point* collisionPoints = new SDL_Point[4]{ {13, 31}, { 26, 39 }, { 13, 46 }, { 0, 39 } }; // 13, 31		26, 39		13, 46		0, 39
	void setupCollision();

	Fcollision* collision;

	int calcAutoFisherId();
};