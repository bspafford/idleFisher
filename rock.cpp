#include "rock.h"

Arock::Arock(vector loc) : Abuilding(loc) {
	buildingImg = new Fimg("./images/landscape/rock.png", loc);

	//setupCollision({ {632,-155}, {698,-187}, {764,-155}, {700,-122} });
	setupCollision({ loc + vector{ 2, 25 }, loc + vector{ 18, 39 }, loc + vector{ 39, 20 }, loc + vector{ 21, 12 } });
}

Arock::~Arock() {

}

void Arock::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);
}