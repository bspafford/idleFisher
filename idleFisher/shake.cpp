#include "shake.h"
#include "timer.h"

Eshake::Eshake(float shakeDist) {
	shakeTimer = std::make_unique<timer>();
	shakeTimer->addCallback(this, &Eshake::setShakeLoc);
	this->shakeDist = shakeDist;
	shakeTimer->start(.1);
}

void Eshake::start(vector loc) {
	shakeLoc = loc;
}

void Eshake::updateShake(vector loc) {
	startLoc = loc;
}

void Eshake::setShakeDist(float shakeDist) {
	this->shakeDist = shakeDist;
}

void Eshake::setShakeLoc() {
	float radius = 5;
	float r = radius * sqrt(math::randRange(0, 1));
	float theta = math::randRange(0, 1) * 2 * M_PI;
	float x = startLoc.x + r * cos(theta);
	float y = startLoc.y + r * sin(theta);
	shakeLoc = { x, y };
	shakeTimer->start(.1);
}

vector Eshake::getShakeLoc() {
	return shakeLoc;
}