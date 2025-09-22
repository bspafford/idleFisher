#include "ship.h"
#include "timer.h"
#include "Image.h"

Aship::Aship(vector loc) {
	this->loc = loc;
	tempLoc = loc;

	shipImg = new Image("./images/ship1.png", loc, true);
	bobTimer = new timer();
	bobTimer->addCallback(this, &Aship::shipbob, &Aship::shipbob);
	bobTimer->start(5);
}

Aship::~Aship() {
	delete shipImg;
	delete bobTimer;
	shipImg = nullptr;
	bobTimer = nullptr;
}

void Aship::draw(Shader* shaderProgram) {
	shipImg->draw(shaderProgram);
}

void Aship::shipbob() {
	float time = bobTimer->getTime();
	float timer = bobTimer->getMaxTime();

	vector bobberY(0, roundf(sin(time / timer * 2 * M_PI) * 2));

	loc = tempLoc + bobberY;
	shipImg->setLoc(loc);

	if (bobTimer->finished()) {
		tempLoc = loc;
		bobTimer->start(5);
	}
}