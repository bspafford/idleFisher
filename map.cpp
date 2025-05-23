#include "map.h"
#include "main.h"
#include "button.h"
#include "text.h"
#include "saveData.h"
#include "currencyWidget.h"
#include "shortNumbers.h"
#include "sailorWidget.h"
#include "animation.h"

Umap::Umap(UsailorWidget* parent) {
	this->parent = parent;

	mapImg = new Fimg("./images/sailorMap.png", {-1073, -739});

	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "anim", {{0, 0}, {9, 0}, .333, true} });
	hereBoat = new animation("widget/hereBoat.png", 22, 22, animData, false, { 0, 0 });
	hereBoat->setAnimation("anim");
	hereBoat->start();
	hereText = new text("Here", "straight", { 0, 0 }, false, false, textAlign::center);

	for (int i = 0; i < worldButtonLoc.size(); i++) {
		worldButtonLoc[i] = worldButtonLoc[i] + vector{ 308, 196 };
	}

	for (int i = 0; i < worldButtonLoc.size(); i++) {
		Ubutton* button = new Ubutton(parent, "x.png", 21, 22, 1, worldButtonLoc[i] * stuff::pixelSize, false, false);
		button->setParent(this);
		worldButtons.push_back(button);
		

		std::string textString = "";
		if (SaveData::saveData.worldList[i].unlocked)
			textString = SaveData::data.worldData[i].name;
		else
			textString = shortNumbers::convert2Short(SaveData::data.worldData[i].currencyNum);
		if (worldButtons.size() > i)
			worldNames.push_back(new text(textString, "straight", worldButtonLoc[i] * stuff::pixelSize + vector{ worldButtons[i]->getSize().x / 2, 25 * stuff::pixelSize }, false, false, textAlign::center));
	}

	for (int i = 0; i < worldButtonLoc.size() - 1; i++) {
		Fimg* line = new Fimg("./images/widget/maps/mapLine" + std::to_string(i + 1) + "-" + std::to_string(i + 2) + ".png");
		worldLines.push_back(line);
	}

	if (worldButtons.size() >= 10) {
		worldButtons[0]->addCallback(this, &Umap::openWorld1);
		worldButtons[1]->addCallback(this, &Umap::openWorld2);
		worldButtons[2]->addCallback(this, &Umap::openWorld3);
		worldButtons[3]->addCallback(this, &Umap::openWorld4);
		worldButtons[4]->addCallback(this, &Umap::openWorld5);
		worldButtons[5]->addCallback(this, &Umap::openWorld6);
		worldButtons[6]->addCallback(this, &Umap::openWorld7);
		worldButtons[7]->addCallback(this, &Umap::openWorld8);
		worldButtons[8]->addCallback(this, &Umap::openWorld9);
		worldButtons[9]->addCallback(this, &Umap::openWorld10);
	}

	setLocs(mapImg->loc); // updates starting pos
}

void Umap::draw(SDL_Renderer* renderer) {
	if (mapImg)
		mapImg->draw(renderer, false);

	for (int i = 0; i < worldLines.size(); i++) {
		if (!SaveData::saveData.worldList[i].unlocked)
			break;

		worldLines[i]->draw(renderer, false);
	}

	for (int i = 0; i < worldButtons.size(); i++) {
		std::string currWorld = Main::currWorld;

		if (i != math::getWorldIndexFromName(Main::currWorld)) {
			worldButtons[i]->draw(renderer);

			if (currWorld != Main::currWorld) // check if the worlds have changed
				return;

			worldNames[i]->draw(renderer);

			if (!SaveData::saveData.worldList[i].unlocked)
				break;
		} else { // draws boat at current world map loc
			hereBoat->setLoc(worldButtons[i]->getLoc());
			hereBoat->draw(renderer);
			hereText->setLoc(worldNames[i]->getLoc());
			hereText->draw(renderer);
		}
	}

	if (Main::bLeftMouseButtonDown) {
		moveMap();
	}
	mouseDown = Main::bLeftMouseButtonDown;
}

void Umap::moveMap() {
	if (!mouseDown) { // first frame down
		imgStartPos = mapImg->loc;
		mouseStartPos = Main::mousePos;
	}

	vector diff = mouseStartPos - imgStartPos;
	vector newLoc = Main::mousePos - diff;

	float x = math::clamp(newLoc.x, Main::screenWidth - mapImg->w * stuff::pixelSize, 0);
	float y = math::clamp(newLoc.y, Main::screenHeight - mapImg->h * stuff::pixelSize, 0);
	setLocs({ x, y });
}

void Umap::setLocs(vector loc) {
	this->ogLoc = parent->mapBackground->loc + vector{ 35, 35 } * stuff::pixelSize;
	mapImg->loc = loc;

	for (int i = 0; i < worldButtons.size(); i++) {
		worldButtons[i]->setLoc(loc + worldButtonLoc[i] * stuff::pixelSize);

		worldNames[i]->setLoc(loc + worldButtonLoc[i] * stuff::pixelSize + vector{ worldButtons[i]->getSize().x / 2, 25 * stuff::pixelSize });
	}

	for (int i = 0; i < worldLines.size(); i++) {
		worldLines[i]->loc = loc + worldLineLoc[i] * stuff::pixelSize;
	}
}

void Umap::openLevel(std::string levelName) {
	// check to see if world unlock
	// if not unlocked
		// buy / unlock it
	// otherwise
		// go there

	// world to id
	FsaveWorldStruct* saveWorld = nullptr;
	FworldStruct* world = nullptr;
	for (int i = 0; i < SaveData::data.worldData.size(); i++) {
		if (SaveData::data.worldData[i].worldName == levelName) {
			world = &SaveData::data.worldData[i];
			saveWorld = &SaveData::saveData.worldList[i];
			break;
		}
	}

	if (!world || !saveWorld)
		return;

	if (saveWorld->unlocked) { // already unlocked
		Main::openLevel(levelName);
	} else if (world->currencyNum <= SaveData::saveData.currencyList[world->currencyId].numOwned) { // can afford
		SaveData::saveData.currencyList[world->currencyId].numOwned -= world->currencyNum;
		saveWorld->unlocked = true;

		// update the map
		worldNames[world->id]->setText(world->name);
		Main::currencyWidget->updateList();
	}

}

void Umap::setLoc(vector loc) {
	__super::setLoc(loc);
	this->loc = loc;
}
	
vector Umap::getLoc() {
	return loc;
}