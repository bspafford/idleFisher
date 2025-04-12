#include "pet.h"
#include "saveData.h"
#include "AStar.h"
#include "main.h"
#include "Acharacter.h"

Apet::Apet(FpetStruct* pet, vector loc) {
	this->pet = pet;
	SaveData::saveData.equippedPet = *pet;
	img = new Fimg("./images/pets/" + std::to_string(pet->id) + ".png");
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
		vector diff = Astar->followPath(loc, deltaTime);
		setLoc(loc + diff * speed);
	}

	//setAnimation();
}

void Apet::draw(SDL_Renderer* renderer) {
	if (!SaveData::settingsData.showPets)
		return;

	Astar->drawBoard(renderer);

	img->draw(renderer);
}

void Apet::setLoc(vector loc) {
	this->loc = loc;
	img->loc = loc - vector{ img->w / 2.f, float(img->h) };
}

FpetStruct* Apet::getPetStruct() {
	return pet;
}