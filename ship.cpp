#include "ship.h"
#include "timer.h"

Aship::Aship(vector loc) {
	this->loc = loc;
	tempLoc = loc;

	shipImg = new Fimg("./images/ship1.png", loc);
	bobTimer = new timer();
	bobTimer->addCallback(this, &Aship::shipbob, &Aship::shipbob);
	bobTimer->start(5);
}

Aship::~Aship() {
	delete shipImg;
	shipImg = nullptr;
}

void Aship::draw(SDL_Renderer* renderer) {
	shipImg->draw(renderer);
}

void Aship::shipbob() {
	float time = bobTimer->getTime();
	float timer = bobTimer->getMaxTime();

	vector bobberY = { 0, round(sin(time / timer * 2 * M_PI) * 2) };

	loc = tempLoc + bobberY;
	shipImg->loc = loc;

	if (bobTimer->finished()) {
		tempLoc = loc;
		bobTimer->start(5);
	}
}