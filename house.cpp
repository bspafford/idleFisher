#include "house.h"
#include "main.h"
#include "button.h"

Ahouse::Ahouse(vector loc) : Abuilding(loc) {
	//buildingImg = new Fimg("./images/house.png", loc);
	buildingImg = new Fimg("./images/house.png", loc);
	door = new Ubutton(NULL, "houseDoor.png", 23, 62, 1, loc + vector{ 34, 73 }, true, true);
	door->addCallback(this, &Ahouse::enterVault);

	setupCollision({ {742,-358}, { 832,-317 }, { 748,-275 }, { 660,-319 } });
}

Ahouse::~Ahouse() {

}

void Ahouse::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	door->draw(renderer);
}

void Ahouse::setLoc(vector loc) {
	__super::setLoc(loc);
	door->setLoc(loc + vector{ 34, 73 });
}

void Ahouse::enterVault() {
	Main::openLevel("vault");
}