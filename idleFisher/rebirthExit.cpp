#include "rebirthExit.h"
#include "button.h"
#include "main.h"

ArebirthExit::ArebirthExit(vector loc) {
	door = std::make_unique<Ubutton>(nullptr, "worlds/rebirth/rebirthExit.png", 54, 111, 1, loc, true, true);
	door->addCallback(this, &ArebirthExit::exitWorld);
}

void ArebirthExit::draw(Shader* shaderProgram) {
	door->draw(shaderProgram);
}

void ArebirthExit::exitWorld() {
	std::cout << "rebirthing!\n"; 
	Main::rebirth();
}