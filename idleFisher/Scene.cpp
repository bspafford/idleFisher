#include "Scene.h"
#include "worlds.h"
#include "saveData.h"
#include "widget.h"
#include "collision.h"
#include "Texture.h"
#include "AStar.h"

void Scene::openLevel(std::string worldName, int worldChangeLoc, bool overrideIfInWorld) {
	// returns if the world is already open
	if (!overrideIfInWorld && worldName == currWorldName)
		return;

	switchingScenes = true;

	if (worldName == "vault" && currWorldName != "vault")
		prevWorld = currWorldName;
	currWorldName = worldName;
	SaveData::saveData.currWorld = currWorldName;

	if (widget::getCurrWidget())
		widget::getCurrWidget()->removeFromViewport();

	collision::getCollisionObjects();

	// deconstruct worlds
	Texture::deleteCache();
	AStar::Deconstructor();
	if (titleScreen::currTitleScreen) {
		delete titleScreen::currTitleScreen;
		titleScreen::currTitleScreen = nullptr;
	}
	if (world::currWorld) {
		delete world::currWorld;
		world::currWorld = nullptr;
	}
	vaultWorld::deconstructor();
	rebirthWorld::deconstructor();

	if (currWorldName == "titleScreen") {
		titleScreen::currTitleScreen = new titleScreen();
		titleScreen::currTitleScreen->start();
	} else if (currWorldName == "vault") {
		vaultWorld::vaultWorld();
		vaultWorld::start();
	} else if (currWorldName == "rebirth") {
		rebirthWorld::rebirthWorld();
		rebirthWorld::start();
	} else if (currWorldName == "world1") {
		world::currWorld = new world1(worldChangeLoc);
	} else if (currWorldName == "world2") {
		world::currWorld = new world2(worldChangeLoc);
	} else if (currWorldName == "world3") {
		world::currWorld = new world3(worldChangeLoc);
	} else if (currWorldName == "world4") {
		world::currWorld = new world4(worldChangeLoc);
	} else if (currWorldName == "world5") {
		world::currWorld = new world5(worldChangeLoc);
	} else if (currWorldName == "world6") {
		world::currWorld = new world6(worldChangeLoc);
	} else if (currWorldName == "world7") {
		world::currWorld = new world7(worldChangeLoc);
	} else if (currWorldName == "world8") {
		world::currWorld = new world8(worldChangeLoc);
	} else if (currWorldName == "world9") {
		world::currWorld = new world9(worldChangeLoc);
	} else if (currWorldName == "world10") {
		world::currWorld = new world10(worldChangeLoc);
	}

	if (world::currWorld)
		world::currWorld->start();

	AStar::init();
}

int Scene::getWorldIndexFromName(std::string worldName) {
	for (int i = 0; i < SaveData::data.worldData.size(); i++)
		if (SaveData::data.worldData[i].worldName == worldName)
			return i;
	return -1;
}

std::string Scene::getPrevWorldName() {
	return prevWorld;
}

std::string Scene::getCurrWorldName() {
	return currWorldName;
}