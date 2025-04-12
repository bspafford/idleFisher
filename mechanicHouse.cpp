#include "mechanicHouse.h"

AmechanicHouse::AmechanicHouse(vector loc) : Abuilding(loc) {
	buildingImg = new Fimg("./images/mechanicHouse.png", loc);

	setupCollision({ {632,-155}, {698,-187}, {764,-155}, {700,-122} });
}

AmechanicHouse::~AmechanicHouse() {

}

void AmechanicHouse::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);
}