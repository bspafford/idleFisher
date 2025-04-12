	#include "worlds.h"
#include "main.h"
#include "Acharacter.h"
#include "AautoFisher.h"
#include "buyAutoFisher.h"
#include "dumpster.h"
#include "vault.h"
#include "vaultPlacedItems.h"
#include "fishSchool.h"
#include "ship.h"
#include "upgrades.h"

#include "button.h"
#include "animation.h"
#include "timer.h"

// npcs
#include "pet.h"
#include "sailor.h"
#include "fisherman.h"
#include "atm.h"
#include "scuba.h"
#include "petSeller.h"
#include "merchant.h"
#include "mechanic.h"
#include "decorator.h"
#include "fishTransporter.h"
#include "rain.h"
#include "tree.h"

// npc buildings
#include "house.h"
#include "merchantShop.h"
#include "mechanicHouse.h"
#include "petShop.h"
#include "rock.h"

// title screen
titleScreen::titleScreen() {
	fishermanDock = new Fimg("./images/worlds/titleScreen/characterDock.png");
	title = new Fimg("./images/worlds/titleScreen/title.png");
	trees = new Fimg("./images/worlds/titleScreen/trees.png");

	newGameButton = new Ubutton(NULL, "widget/pauseMenu/newGame.png", 64, 19, 1, {45 * stuff::pixelSize, 130 * stuff::pixelSize}, false, false);
	newGameButton->addCallback(newGame);

	continueButton = new Ubutton(NULL, "widget/pauseMenu/continue.png", 67, 19, 1, { 45 * stuff::pixelSize, 162  * stuff::pixelSize }, false, false);
	continueButton->addCallback(continueGame);

	exitButton = new Ubutton(NULL, "widget/pauseMenu/exit.png", 35, 19, 1, { 45 * stuff::pixelSize, 194 * stuff::pixelSize }, false, false);
	exitButton->addCallback(exit);
}

void titleScreen::deconstructor() {
	delete fishermanDock;
	delete title;
	delete trees;
	delete newGameButton;
	delete continueButton;
	delete exitButton;
	fishermanDock = nullptr;
	title = nullptr;
	trees = nullptr;
	newGameButton = nullptr;
	continueButton = nullptr;
	exitButton = nullptr;
}

void titleScreen::start() {

}

void titleScreen::newGame() {
	fadeTimer = new timer();
	fadeTimer->addUpdateCallback(titleScreen::fadeToBlack);
	fadeTimer->start(.3);
	fadeTimer->addCallback(openWorld);

	// temp
	// Main::currWorld = "world1";
}

void titleScreen::fadeToBlack() {
	alpha = fadeTimer->getTime() / fadeTimer->getMaxTime() * 255.f;
}

void titleScreen::openWorld() {
	Main::openLevel(SaveData::saveData.currWorld);
	alpha = 0;
}

void titleScreen::continueGame() {
	// temp
	Main::openLevel("world1");
}

void titleScreen::exit() {
	Main::running = false;
}

void titleScreen::draw(SDL_Renderer* renderer) {
	// set bottom right
	fishermanDock->loc = Main::getScreenSize() - fishermanDock->getSize();
	// set top left
	title->loc = { 0, 0 };
	// set top right
	trees->loc = { Main::getScreenSize().x - trees->getSize().x, 0 };

	trees->draw(renderer, false);
	fishermanDock->draw(renderer, false);
	title->draw(renderer, false);

	newGameButton->draw(renderer);
	continueButton->draw(renderer);
	if (Main::currWorld != "titleScreen")
		return;
	exitButton->draw(renderer);

	SDL_SetRenderDrawColor(Main::renderer, 18, 11, 22, alpha);
	SDL_Rect rect = {0, 0, Main::screenWidth, Main::screenHeight };
	SDL_RenderFillRect(Main::renderer, &rect);
}

vaultWorld::vaultWorld() {
	map = new Fimg("./images/vaultMap.png", { 20, -210 });
	houseDoor = new Ubutton(NULL, "houseDoor.png", 23, 62, 1, { 115, -13 }, true, true);
	houseDoor->addCallback(vaultWorld::leaveHouse);

	decorator = new Adecorator({ 50, -10 });

	AvaultPlacedItems();
}

void vaultWorld::deconstructor() {
	delete map;
	delete houseDoor;
	delete decorator;
	map = nullptr;
	houseDoor = nullptr;
	decorator = nullptr;
}

void vaultWorld::start() {
	SaveData::saveData.playerLoc = { 546, 46 }; // 550, 55
	//circleAnim->start();
}

void vaultWorld::leaveHouse() {
	Main::openLevel(SaveData::saveData.prevWorld, worldLoc::leaveVault);
}

void vaultWorld::draw(SDL_Renderer* renderer) {
	// set background color black
	SDL_Rect rect = { 0, 0, Main::screenWidth, Main::screenHeight };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

	map->draw(renderer);
	if (Main::currWorld != "vault")
		return; // if clicked on vault to change worlds

	AvaultPlacedItems::draw(renderer);
	decorator->draw(renderer);


	//Main::character->anim->setLoc({ Main::screenWidth / 2 - (Main::character->anim->cellWidth / 2) * stuff::pixelSize, Main::screenHeight / 2 - Main::character->anim->cellHeight / 2 * stuff::pixelSize });
	Main::character->draw(renderer);

	houseDoor->draw(renderer);
	decorator->calcIfPlayerInfront();

	Main::drawWidgets(renderer);
}

rebirthWorld::rebirthWorld() {

}

void rebirthWorld::deconstructor() {

}

void rebirthWorld::start() {

}

void rebirthWorld::draw(SDL_Renderer* renderer) {
	Main::character->draw(renderer);
}

world::world() {
	rain = new Arain();
	rainStartTimer = new timer();
	rainStartTimer->addCallback(&world::startRain);
	rainDurationTimer = new timer();
	rainDurationTimer->addCallback(&world::stopRain);
	darkenScreenTimer = new timer();
	darkenScreenTimer->addUpdateCallback(&world::darkenScreen);
	darkenValue = 0;
	isRaining = false;

	rainStartTimer->start(math::randRange(upgrades::calcMinRainSpawnInterval(), upgrades::calcMaxRainSpawnInterval()));
}

void world::startRain() {
	isRaining = true;
	darkenScreenTimer->start(1);
	rain->start();

	rainDurationTimer->start(math::randRange(minRainDurationTime, maxRainDurationTime));
}

void world::stopRain() {
	isRaining = false;
	darkenScreenTimer->start(1);
	rain->stop();

	rainStartTimer->start(math::randRange(upgrades::calcMinRainSpawnInterval(), upgrades::calcMaxRainSpawnInterval()));
}

void world::darkenScreen() {
	if (isRaining) // get darker
		darkenValue = darkenScreenTimer->getTime() / darkenScreenTimer->getMaxTime() * 100;
	else // brighten up
		darkenValue = (1 - darkenScreenTimer->getTime() / darkenScreenTimer->getMaxTime()) * 100;
}

void world::deconstructor() {
	delete mapAnim;
	delete mapImg;
	delete inFront;
	delete circle;
	delete circleAnim;
	delete buyer;
	delete sellFish;
	// npcs
	delete sailor;
	delete fisherman;
	delete atm;
	delete scuba;
	delete petSeller;
	delete merchant;
	delete mechanic;
	delete fishTransporter;
	// buildings
	for (int i = 0; i < buildingList.size(); i++)
		delete buildingList[i];
	buildingList.clear();
	
	npcList.clear();
	for (int i = 0; i < fishSchoolList.size(); i++)
		delete fishSchoolList[i];
	fishSchoolList.clear();

	mapAnim = nullptr;
	mapImg = nullptr;
	inFront = nullptr;
	circle = nullptr;
	circleAnim = nullptr;
	buyer = nullptr;
	sellFish = nullptr;
	sailor = nullptr;
	fisherman = nullptr;
	atm = nullptr;
	scuba = nullptr;
	petSeller = nullptr;
	merchant = nullptr;
	mechanic = nullptr;
	fishTransporter = nullptr;
	merchantShop = nullptr;
	house = nullptr;
	mechanicHouse = nullptr;
	petShop = nullptr;
}

void world::start() {
	// on init make the circle appear
	//circleAnim->start();
	fishSchoolSpawnTimer = new timer();
	fishSchoolSpawnTimer->addCallback(&world::spawnFishSchool);
	fishSchoolSpawnTimer->start(math::randRange(upgrades::calcMinFishSchoolSpawnInterval(), upgrades::calcMaxFishSchoolSpawnInterval()));
}

void world::spawnFishSchool() {
	if (fishSchoolList.size() < maxFishSchoolNum) { // only spawn school if less than max
		float x = math::randRange(-200, 450);
		float y = math::randRange(-200, 150);
		AfishSchool* fishSchool = new AfishSchool({ x, y });
		fishSchoolList.push_back(fishSchool);
	}
	fishSchoolSpawnTimer->start(math::randRange(upgrades::calcMinFishSchoolSpawnInterval(), upgrades::calcMaxFishSchoolSpawnInterval()));
}

void world::removeFishSchool(AfishSchool* fishSchool) {
	auto it = std::find(fishSchoolList.begin(), fishSchoolList.end(), fishSchool);
	if (it != fishSchoolList.end())
		fishSchoolList.erase(it);
}

void world::makeDrawLists() {
	npcList.clear();
	buildingList.clear();

	// npcs
	npcList.push_back(sailor);
	npcList.push_back(fisherman);
	npcList.push_back(atm);
	npcList.push_back(scuba);
	npcList.push_back(petSeller);
	npcList.push_back(merchant);
	npcList.push_back(mechanic);
	npcList.push_back(fishTransporter);

	// buildings
	if (house)
		buildingList.push_back(house);
	if (merchantShop)
		buildingList.push_back(merchantShop);
	if (mechanicHouse)
		buildingList.push_back(mechanicHouse);
	if (petShop)
		buildingList.push_back(petShop);
}

void world::draw(SDL_Renderer* renderer) {
	makeDrawLists();

	// draw background
	mapImg->draw(renderer);
	if (ship)
		ship->draw(renderer);
	//vault->draw(renderer);
	if (Main::currWorld == "vault")
		return; // if clicked on vault

	// temp
	//if (merchantShop)
	//	merchantShop->draw(renderer);

	for (int i = 0; i < fishSchoolList.size(); i++)
		fishSchoolList[i]->draw(renderer);

	for (int i = 0; i < trees.size(); i++)
		trees[i]->draw(renderer);

	sellFish->draw(renderer);
	if (Main::pet)
		Main::pet->draw(renderer);
	sortDraw(renderer);

	if (rain) {
		rain->draw(renderer);
		// makes screen darker
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, darkenValue);
		SDL_Rect rect = { 0, 0, Main::screenWidth, Main::screenHeight };
		SDL_RenderFillRect(renderer, &rect);
	}

	Main::drawWidgets(renderer);

	if (circle && circleAnim && !circleAnim->finished())
		circle->draw(renderer, false);
}

void world::sortDraw(SDL_Renderer* renderer) {
	// temp
	// should have some sort of big list that sorts all the dynamic and static accordingly
	// shouldn't have it broken up into a bunch of different lists

	// sort all the draws to render some infront or behind depending on location

	// get base of image, position + imgH
	// if base of image > other base of image
		// render after


	// find the the lowest pixel in the far left side of the image, and do the same for the right?
	// then use those two location to test against the player

	vector loc;
	if (Main::character->anim) {
		//Main::character->anim->setLoc({Main::screenWidth / 2 - (Main::character->anim->cellWidth / 2) * stuff::pixelSize, Main::screenHeight / 2 - Main::character->anim->cellHeight / 2 * stuff::pixelSize});
		//loc = math::screenToWorld(Main::screenWidth / 2 + Main::character->anim->cellWidth / 2 * stuff::pixelSize, Main::screenHeight / 2 + Main::character->anim->cellHeight / 2 * stuff::pixelSize);
	}

	//vector screenLoc = math::worldToScreen(loc);
	vector screenLoc = math::worldToScreen(SaveData::saveData.playerLoc);

	std::vector<AautoFisher*> autoFishersBehind;
	std::vector<AautoFisher*> autoFishersInFront;
	std::vector<npc*> npcBehind;
	std::vector<npc*> npcInFront;
	std::vector<Abuilding*> buildingBehind;
	std::vector<Abuilding*> buildingInFront;

	// calc autofisher
	for (int i = 0; i < autoFisherList.size(); i++) {
		if (autoFisherList[i]->inFrontPlayer)
			autoFishersInFront.push_back(autoFisherList[i]);
		else
			autoFishersBehind.push_back(autoFisherList[i]);
	}

	// calc npc
	for (npc* _npc : npcList) {
		if (!_npc)
			continue;
		if (_npc->calcIfPlayerInfront())
			npcBehind.push_back(_npc);
		else
			npcInFront.push_back(_npc);
	}

	for (Abuilding* building : buildingList) {
		if (building->calcInFront())
			buildingInFront.push_back(building);
		else
			buildingBehind.push_back(building);
	}

	for (int i = 0; i < buildingBehind.size(); i++)
		buildingBehind[i]->draw(renderer);

	// draw npcs behind
	for (int i = 0; i < npcBehind.size(); i++) {
		npcBehind[i]->draw(renderer);
	}

	// draw autofishers behind
	for (int i = 0; i < autoFishersBehind.size(); i++) {
		autoFishersBehind[i]->draw(renderer);
	}

	if (!buyer->inFrontPlayer)
		buyer->draw(renderer);

	// if bobber above player, render behind
	if (Main::character->tempBobberLoc.y > Main::character->getCharScreenLoc().y) {
		Main::character->draw(renderer);
		Main::character->drawFishingLine(renderer);
	} else {
		Main::character->drawFishingLine(renderer);
		Main::character->draw(renderer);
	}

	if (buyer->inFrontPlayer)
		buyer->draw(renderer);

	for (int i = 0; i < buildingInFront.size(); i++)
		buildingInFront[i]->draw(renderer);

	// draw npcs infront
	for (int i = 0; i < npcInFront.size(); i++) {
		npcInFront[i]->draw(renderer);
	}

	// draw autofishers infront
	for (int i = 0; i < autoFishersInFront.size(); i++) {
		autoFishersInFront[i]->draw(renderer);
	}

	if (inFront)
		inFront->draw(renderer);
}

void world::setupAutoFishers() {
	autoFisherList.clear();
	// load autoFisher
	for (int i = 0; i < SaveData::saveData.autoFisherList.size(); i++) {
		if (SaveData::saveData.autoFisherList[i].unlocked && SaveData::data.autoFisherData[i].worldName == Main::currWorld) {
			AautoFisher* autoFisher = new AautoFisher(i);
			autoFisherList.push_back(autoFisher);
		}
	}
}

void world::setWorldChangeLoc(int worldChangeLoc) {
	if (worldChangeLoc == worldLoc::changeWorlds)
		SaveData::saveData.playerLoc = spawnLoc;
	else if (worldChangeLoc == worldLoc::leaveVault) {
		SaveData::saveData.playerLoc = houseLoc;
	}
}

// world 1
world1::world1(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	//mapImg = new Fimg("./images/worlds/world1/map1 - Copy (3).png", { -500, -775 });
	mapImg = new Fimg("./images/worlds/world1/map1 - Copy (5)-export.png", { -500, -775 });
	for (int i = 0; i < 19; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	inFront = new Fimg("./images/worlds/world1/dockInfront.png", { -500, -500 });
	ship = new Aship({ -164, -237 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png"};
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();
	
	buyer = new buyAutoFisher({295, -170});

	sellFish = new dumpster({ 739-500, 546-775 });

	// npcs
	sailor = new Asailor({ 10, 5 });
	fisherman = new Afisherman({ 143+8, -190 }); // 334, -157
	atm = new Aatm({ 327, -186 });
	scuba = new Ascuba({ 354-105, -107+240 });
	petSeller = new ApetSeller({ 365+100, -215-40 });
	merchant = new Amerchant({ 143+410, -190-80 });
	mechanic = new Amechanic({ 560+60, -190+40 });

	if (SaveData::saveData.mechanicStruct[0].unlocked)
		fishTransporter = new AfishTransporter({ 417-140, -157 }); // { 406, -209 }

	// npc buildings
	//merchantShop = new Fimg("./images/merchantShop.png", { 570, -280 });
	house = new Ahouse({1157-500, 71-500});
	merchantShop = new AmerchantShop({ 998-500, 132 - 500 });
	mechanicHouse = new AmechanicHouse({ 1130 - 500, 249 - 500 }); // { 1086 - 500, 285 - 500 }
	petShop = new ApetShop({ 930 - 500, 179 - 500 });

	std::vector<vector> rockLocs = { { 866, 524 }, { 1068, 546 }, { 1379, 556 } };
	for (int i = 0; i < rockLocs.size(); i++) {
		Arock* rock = new Arock(rockLocs[i] - vector{ 500, 775 });
		buildingList.push_back(rock);
	}

	// make trees
	std::vector<vector> treeLocs = { {624, -524},{739, -514},{842, -502},{944, -485},{229, -480},{415, -477},{525, -477},{1101, -460},{663, -454},{310, -448},{114, -433},{475, -432},{773, -426},{574, -425},{1012, -425},{869, -422},{709, -409},{380, -406},{215, -402},{935, -385},{622, -384},{1155, -372},{803, -371},{1063, -368},{546, -367},{288, -349},{461, -338},{373, -332},{869, -330},{1000, -323},{231, -298},{331, -279},{922, -279},{988, -247},{910, -224} };
	std::vector<vector> bushLocs = { {736, -434},{166, -409},{792, -392},{528, -380},{609, -369},{408, -365},{266, -332},{955, -239} };

	for (int i = 0; i < treeLocs.size(); i++) {
		Atree* tree = new Atree(treeLocs[i], true);
		trees.push_back(tree);
	}
	for (int i = 0; i < bushLocs.size(); i++) {
		Atree* bush = new Atree(bushLocs[i], false);
		trees.push_back(bush);
	}
	sortTreeList();

	makeDrawLists();
}

void world::sortTreeList() {
	std::vector<Atree*> sortedList;
	int treeSize = trees.size();
	for (int j = 0; j < treeSize; j++) {
		Atree* curr = trees[0];
		trees.erase(trees.begin());
		bool added = false;
		for (int i = 0; i < sortedList.size(); i++) {
			if (curr->loc.y <= sortedList[i]->loc.y) {
				// insert at i
				sortedList.insert(sortedList.begin() + i, curr);
				added = true;
				break;
			}
		}
		if (!added)
			sortedList.push_back(curr);
	}
	trees = sortedList;
}

world2::world2(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world2/map1.png", { -500, -500 });
	inFront = new Fimg("./images/dockInfront.png", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 405, -256 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 406, -159 });
	makeDrawLists();
}

world3::world3(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world3/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

// world 1
world4::world4(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world4/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world5::world5(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world5/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world6::world6(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world6/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world7::world7(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world7/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world7/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world8::world8(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world8/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world9::world9(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world9/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}

world10::world10(int worldChangeLoc) {
	spawnLoc = { 200, -84 };
	houseLoc = { 1670, -870 }; // { 700, -1400 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Fimg("./images/worlds/world10/map1.png", { -500, -500 });
	inFront = new Fimg("", { -500, -500 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Fimg(circlePath + "1.png");
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();

	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 430, -320 });

	// npcs
	sailor = new Asailor({ 0, -45 });
	fisherman = new Afisherman({ -60, -8 }); // 334, -157
	atm = new Aatm({ 317, -236 });
	scuba = new Ascuba({ 334, -147 });
	petSeller = new ApetSeller({ 355, -265 });
	merchant = new Amerchant({ 133, -240 });
	mechanic = new Amechanic({ 450, -240 });
	fishTransporter = new AfishTransporter({ 417, -157 }); // { 406, -209 }

	makeDrawLists();
}