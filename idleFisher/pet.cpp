#include "pet.h"
#include "saveData.h"
#include "AStar.h"
#include "main.h"
#include "character.h"

Apet::Apet(FpetStruct* pet, vector loc) {
	this->pet = pet;
	SaveData::saveData.equippedPet = *pet;
	img = new Image("./images/pets/" + std::to_string(pet->id) + ".png", { 0, 0 }, false);
	setLoc(loc);

	Astar = new AStar();
	//Astar->startPathFinding(loc, vector{ 15, 15 });
}

Apet::~Apet() {
	delete Astar;
	delete img;
	Astar = nullptr;
	img = nullptr;
	SaveData::saveData.equippedPet.id = -1;
}

void Apet::update(float deltaTime) {

	// get if character is inside a valid spot
	if (Astar->nodeFromWorldPoint(Main::character->getCharLoc())->walkable)
		Astar->startPathFinding(loc, Main::character->getCharLoc());
	
	if (Astar->followingPath) {
		//vector diff = math::normalize(SaveData::saveData.playerLoc / stuff::pixelSize - loc) * deltaTime; // directly follow the player
		vector diff = Astar->followPath(loc, deltaTime, speed);
		setLoc(loc + diff * speed);
	}

	//setAnimation();
}

void Apet::draw(Shader* shaderProgram) {
	if (!SaveData::settingsData.showPets)
		return;

	Astar->drawBoard(shaderProgram);

	img->draw(shaderProgram);
}

void Apet::setLoc(vector loc) {
	this->loc = loc;
	img->setLoc(loc - vector{ img->w / 2.f, float(img->h) });
}

FpetStruct* Apet::getPetStruct() {
	return pet;
}