#include "house.h"
#include "main.h"
#include "button.h"

Ahouse::Ahouse(vector loc) : Abuilding(loc) {
	//buildingImg = new Image("./images/house.png", loc);
	buildingImg = new Image("./images/house.png", loc, true);
	door = new Ubutton(NULL, "houseDoor.png", 23, 62, 1, loc + vector{ 34, 19 }, true, true);
	door->addCallback(this, &Ahouse::enterVault);

	//setupCollision({ {742,-358}, { 832,-317 }, { 748,-275 }, { 660,-319 } });
	setupCollision({ loc + vector{3, 44}, loc + vector{91, 0}, loc + vector{175, 42}, loc + vector{77, 86} });
}

Ahouse::~Ahouse() {

}

void Ahouse::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);

	door->draw(shaderProgram);
}

void Ahouse::setLoc(vector loc) {
	__super::setLoc(loc);
	door->setLoc(loc + vector{ 34, 73 });
}

void Ahouse::enterVault() {
	Main::openLevel("vault");
}