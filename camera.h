#pragma once

#include "math.h"

struct structTest {
	int id = -1;
	bool unlocked = false;
	double numOwned = 0;
	double totalNumOwned = 0;
	bool stars[3] = { false, false, false };
};

class Ucamera {
public:
	Ucamera();
	void Update(float deltaTime);
	void detachCamera();

	vector pos;

private:
	vector velocity;
	bool isCameraAttached = true;
};