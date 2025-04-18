#pragma once

#include "math.h"

class timer;

class Eshake {
public:
	Eshake(float shakeDist);
	void updateShake(vector loc);
	void setShakeDist(float shakeDist);
	vector getShakeLoc();
	void start(vector loc);

private:
	// called when the timer is finished
	void setShakeLoc();

	float shakeDist;
	vector startLoc;
	vector shakeLoc;

	timer* shakeTimer;
};