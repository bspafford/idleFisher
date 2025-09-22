#include "worlds.h"
#include "main.h"
#include "character.h"
#include "AautoFisher.h"
#include "buyAutoFisher.h"
#include "dumpster.h"
#include "vaultPlacedItems.h"
#include "fishSchool.h"
#include "ship.h"
#include "upgrades.h"
#include "Image.h"
#include "Texture.h"
#include "Rectangle.h"

#include "button.h"
#include "animation.h"
#include "timer.h"
#include "rebirthExit.h"

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
#include "fishGod.h"

// npc buildings
#include "house.h"
#include "merchantShop.h"
#include "mechanicHouse.h"
#include "petShop.h"
#include "rock.h"
#include "rain.h"
#include "tree.h"

// title screen
titleScreen::titleScreen() {
	//waterImg = new Image("./images/water/water.png", { 100, -100 }, false);
	waterImg = new Image("./images/worlds/titleScreen/depthMap.png", { 0, 0 }, false);

	//fishermanDock = new Image("./images/worlds/titleScreen/characterDock.png", { 0, 0 }, false);
	std::unordered_map<std::string, animDataStruct> fishermanDockAnimData;
	fishermanDockAnimData.insert({ "anim", { {0, 0}, {48, 0}, .1, true } });
	fishermanDock = new animation("worlds/titleScreen/characterDockSpriteSheet.png", 403, 303, fishermanDockAnimData, false);
	fishermanDock->setAnimation("anim");
	fishermanDock->start();
	title = new Image("./images/worlds/titleScreen/title.png", { 0, 0 }, false);

	std::unordered_map<std::string, animDataStruct> treesAnimData;
	treesAnimData.insert({ "anim", {{0, 0}, {14, 0}, .1, true} });
	trees = new animation("worlds/titleScreen/treesSpriteSheet.png", 337, 96, treesAnimData, false);
	trees->setAnimation("anim");
	trees->start();

	newGameButton = new Ubutton(NULL, "widget/pauseMenu/newGame.png", 64, 19, 1, {45 * stuff::pixelSize, 150 * stuff::pixelSize}, false, false);
	newGameButton->addCallback(newGame);

	continueButton = new Ubutton(NULL, "widget/pauseMenu/continue.png", 67, 19, 1, { 45 * stuff::pixelSize, 182  * stuff::pixelSize }, false, false);
	continueButton->addCallback(continueGame);

	exitButton = new Ubutton(NULL, "widget/pauseMenu/exit.png", 35, 19, 1, { 45 * stuff::pixelSize, 214 * stuff::pixelSize }, false, false);
	exitButton->addCallback(exit);

	transitionBox = new URectangle({ 0, 0 }, stuff::screenSize, false, glm::vec4(0.f));
}

void titleScreen::deconstructor() {
	delete waterImg;
	delete fishermanDock;
	delete title;
	delete trees;
	delete newGameButton;
	delete continueButton;
	delete exitButton;
	waterImg = nullptr;
	fishermanDock = nullptr;
	title = nullptr;
	trees = nullptr;
	newGameButton = nullptr;
	continueButton = nullptr;
	exitButton = nullptr;
}

void titleScreen::start() {
	// setup water shader images
	Texture::bindTextureToShader({ Main::waterShader, Main::twoDWaterShader }, "./images/water/waterDUDV.png", "dudvMap");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/titleScreen/water.png", "underwaterTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/causticTexture.png", "causticTexture");
	Texture::bindTextureToShader({ Main::twoDWaterShader, Main::twoDShader }, "./images/worlds/titleScreen/depthMap.png", "waterDepthTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/titleScreen/reflections.png", "reflectionTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "", "underwaterObjectTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "", "underwaterObjectDepthMap");
	Main::twoDWaterShader->setVec3("deepWaterColor", glm::vec3(54.f / 255.f, 107.f / 255.f, 138.f / 255.f));
	Main::twoDWaterShader->setVec3("shallowWaterColor", glm::vec3(206.f / 255.f, 210.f / 255.f, 158.f / 255.f));
	Main::twoDWaterShader->setFloat("causticSize", 6.f);
}

void titleScreen::newGame() {
	fadeTimer = new timer();
	fadeTimer->addUpdateCallback(titleScreen::fadeToBlack);
	fadeTimer->start(.3);
	fadeTimer->addCallback(openWorld);

	// temp
	// Main::currWorld = "world1";
}

void titleScreen::continueGame() {
	fadeTimer = new timer();
	fadeTimer->addUpdateCallback(titleScreen::fadeToBlack);
	fadeTimer->addCallback(openWorld);
	fadeTimer->start(.3);
}

void titleScreen::fadeToBlack() {
	alpha = fadeTimer->getTime() / fadeTimer->getMaxTime();
	transitionBox->setColor(glm::vec4(18 / 255.f, 11.f / 255.f, 22.f / 255.f, alpha));
}

void titleScreen::openWorld() {
	fadeToBlack(); // makes sure it goes to alpha 100% instead of like 99%
	Main::openLevel("world1");
	alpha = 0;
}

void titleScreen::exit() {
	glfwSetWindowShouldClose(Main::window, true);
}

void titleScreen::draw(Shader* shaderProgram) {
	if (waterImg)
		waterImg->draw(Main::twoDWaterShader); // Main::twoDWaterShader

	// set bottom right
	if (fishermanDock)
		fishermanDock->setLoc(stuff::screenSize - vector{ float(fishermanDock->cellWidth), float(fishermanDock->cellHeight) } * stuff::pixelSize);
	// set top left
	if (title)
		title->setLoc({0, 0});
	// set top right
	if (trees)
		trees->setLoc({ stuff::screenSize.x - trees->cellWidth * stuff::pixelSize, 0 });

	if (trees)
		trees->draw(shaderProgram);
	if (fishermanDock)
		fishermanDock->draw(shaderProgram);
	if (title)
		title->draw(shaderProgram);

	if (newGameButton)
		newGameButton->draw(shaderProgram);
	if (continueButton)
		continueButton->draw(shaderProgram);
	if (Main::currWorld != "titleScreen")
		return;

	if (exitButton)
		exitButton->draw(shaderProgram);

	if (transitionBox)
	transitionBox->draw(shaderProgram);
}

vaultWorld::vaultWorld() {
	map = new Image("./images/vaultMap.png", { 20, -210 }, true);
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

void vaultWorld::draw(Shader* shaderProgram) {
	// set background color black
	/*
	SDL_Rect rect = { 0, 0, stuff::screenSize.x, stuff::screenSize.y };
	SDL_SetRenderDrawColor(shaderProgram, 0, 0, 0, 255);
	SDL_RenderFillRect(shaderProgram, &rect);
	*/

	map->draw(shaderProgram);
	if (Main::currWorld != "vault")
		return; // if clicked on vault to change worlds

	AvaultPlacedItems::draw(shaderProgram);
	decorator->draw(shaderProgram);


	//Main::character->anim->setLoc({ stuff::screenSize.x / 2 - (Main::character->anim->cellWidth / 2) * stuff::pixelSize, stuff::screenSize.y / 2 - Main::character->anim->cellHeight / 2 * stuff::pixelSize });
	Main::character->draw(shaderProgram);

	houseDoor->draw(shaderProgram);
	decorator->calcIfPlayerInfront();

	Main::drawWidgets(shaderProgram);
}

rebirthWorld::rebirthWorld() {
	SaveData::saveData.playerLoc = { 0, 0 };
	fishGod = new AfishGod({ 570, 700 });

	rebirthWorldImg = new Image("./images/worlds/rebirth/rebirthWorld.png", vector{ 0, 0 }, true);

	waterTimer = new timer();
	waterTimer->addCallback(rebirthWorld::addAnim);
	waterTimer->start(.13f * 3.f);

	rebirthExit = new ArebirthExit({ 600, 375 });

	doorGlow = new Image("./images/worlds/rebirth/doorGlow.png", { 750-99, 156 }, true);

	waterImg = new Image("./images/worlds/rebirth/depthMap.png", { 0, 0 }, true);
}

void rebirthWorld::deconstructor() {
	delete waterTimer;
	delete fishGod;
	delete rebirthWorldImg;
	delete rebirthExit;
	delete doorGlow;
	delete waterImg;

	waterTimer = nullptr;
	fishGod = nullptr;
	rebirthWorldImg = nullptr;
	rebirthExit = nullptr;
	doorGlow = nullptr;
	waterImg = nullptr;

	for (int i = 0; i < waterRippleList.size(); i++)
		delete waterRippleList[i];
	waterRippleList.clear();
}

void rebirthWorld::start() {
	// setup water shader images
	Texture::bindTextureToShader({ Main::waterShader, Main::twoDWaterShader }, "./images/water/waterDUDV.png", "dudvMap");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/rebirth/water.png", "underwaterTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/causticTexture.png", "causticTexture");
	Texture::bindTextureToShader({ Main::twoDWaterShader, Main::twoDShader }, "./images/worlds/rebirth/depthMap.png", "waterDepthTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/rebirth/reflectionTexture.png", "reflectionTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/rebirth/underwaterObjectTexture.png", "underwaterObjectTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/worlds/rebirth/underwaterObjectDepthMap.png", "underwaterObjectDepthMap");
	Main::twoDWaterShader->setVec3("deepWaterColor", glm::vec3(0, 64.f/255.f, 81.f/255.f));
	Main::twoDWaterShader->setVec3("shallowWaterColor", glm::vec3(0, 130.f/255.f, 121.f/255.f));
	Main::twoDWaterShader->setFloat("causticSize", 16.f);
}

void rebirthWorld::draw(Shader* shaderProgram) {
	waterImg->draw(Main::twoDWaterShader);

	shaderProgram->Activate();
	
	charMoveDir = Main::character->moveDir;

	//rebirthWorldImg->draw(shaderProgram);

	rebirthExit->draw(shaderProgram);
	fishGod->draw(shaderProgram);

	for (int i = 0; i < waterRippleList.size(); i++)
		waterRippleList[i]->draw(shaderProgram);
	Main::character->draw(shaderProgram);

	removeAnim();

	// if movement goes from 0 to !0 then stop timer and set it to .13f * 3.f
	if (math::length(charPrevMoveDir) == 0 && math::length(charMoveDir) != 0 && waterTimer->getMaxTime() != .13f * 3.f) {
		waterTimer->stop();
		waterTimer->start(.13f * 3.f);
	}

	charPrevMoveDir = Main::character->moveDir;
}

void rebirthWorld::addAnim() {
	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "anim", {{0, 0}, {6, 0}, .1, false} });
	animation* waterRipples = new animation("walkRipples.png", 48, 26, animData, true, SaveData::saveData.playerLoc - vector{12, 8}); // Acharacter::getCharLoc()
	waterRipples->setAnimation("anim");
	waterRipples->start();
	waterRippleList.push_back(waterRipples);
	if (math::length(Main::character->moveDir) != 0)
		waterTimer->start(.13f * 3.f);
	else
		waterTimer->start(.13f * 6.f);
}

void rebirthWorld::removeAnim() {
	for (int i = waterRippleList.size() - 1; i >= 0; i--) {
		if (waterRippleList[i]->finished()) {
			waterRippleList[i]->stop();
			waterRippleList.erase(waterRippleList.begin() + i);
		}
	}
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

	std::unordered_map<std::string, animDataStruct> circleAnimData;
	circleAnimData.insert({ "anim", {{0, 0}, {3, 0}, .1, false} });
	circleAnim = new animation("circle/circleSpriteSheet.png", 640, 360, circleAnimData, false);
	circleAnim->setAnimation("anim");

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
	/*
	delete mapAnim;
	delete mapImg;
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

	for (int i = 0; i < poleList.size(); i++)
		delete poleList[i];
	poleList.clear();

	mapAnim = nullptr;
	mapImg = nullptr;
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
	*/

	
	delete mapImg;
	delete mapAnim;
	delete ship;
	delete sailor;
	delete fisherman;
	delete atm;
	delete scuba;
	delete petSeller;
	delete merchant;
	delete mechanic;
	delete fishTransporter;
	delete house;
	delete merchantShop;
	delete mechanicHouse;
	delete petShop;
	delete circle;
	delete circleAnim;
	delete buyer;
	delete sellFish;
	delete fishSchoolSpawnTimer;
	delete rain;
	delete rainStartTimer;
	delete rainDurationTimer;
	delete darkenScreenTimer;
	delete waterImg;
	delete beachAnim;

	mapImg = nullptr;
	mapAnim = nullptr;
	ship = nullptr;
	sailor = nullptr;
	fisherman = nullptr;
	atm = nullptr;
	scuba = nullptr;
	petSeller = nullptr;
	merchant = nullptr;
	mechanic = nullptr;
	fishTransporter = nullptr;
	house = nullptr;
	merchantShop = nullptr;
	mechanicHouse = nullptr;
	petShop = nullptr;
	circle = nullptr;
	circleAnim = nullptr;
	buyer = nullptr;
	sellFish = nullptr;
	fishSchoolSpawnTimer = nullptr;
	rain = nullptr;
	rainStartTimer = nullptr;
	rainDurationTimer = nullptr;
	darkenScreenTimer = nullptr;
	waterImg = nullptr;
	beachAnim = nullptr;

	npcList.clear();
	buildingList.clear();
	for (int i = 0; i < fishSchoolList.size(); i++)
		if (fishSchoolList[i])
			delete fishSchoolList[i];
	fishSchoolList.clear();
	for (int i = 0; i < trees.size(); i++)
		if (trees[i])
			delete trees[i];
	trees.clear();
	for (int i = 0; i < autoFisherList.size(); i++)
		if (autoFisherList[i])
			delete autoFisherList[i];
	autoFisherList.clear();
	for (int i = 0; i < poleList.size(); i++)
		if (poleList[i])
			delete poleList[i];
	poleList.clear();
}

void world::start() {
	// on init make the circle appear
	circleAnim->start();
	fishSchoolSpawnTimer = new timer();
	fishSchoolSpawnTimer->addCallback(&world::spawnFishSchool);
	fishSchoolSpawnTimer->start(math::randRange(upgrades::calcMinFishSchoolSpawnInterval(), upgrades::calcMaxFishSchoolSpawnInterval()));

	// bind texture stuff for water
	Texture::bindTextureToShader({ Main::waterShader, Main::twoDWaterShader }, "./images/water/waterDUDV.png", "dudvMap");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/water.png", "underwaterTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/causticTexture.png", "causticTexture");
	Texture::bindTextureToShader({ Main::twoDWaterShader, Main::twoDShader }, "./images/water/waterDepthMap.png", "waterDepthTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/reflectionTexture.png", "reflectionTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/underwaterObjectTexture.png", "underwaterObjectTexture");
	Texture::bindTextureToShader(Main::twoDWaterShader, "./images/water/underwaterObjectDepthMap.png", "underwaterObjectDepthMap");
	Main::twoDWaterShader->setVec3("deepWaterColor", glm::vec3(54.f/255.f, 107.f/255.f, 138.f/255.f));
	//Main::twoDWaterShader->setVec3("shallowWaterColor", glm::vec3(152.f / 255.f, 226.f / 255.f, 215.f / 255.f));
	//Main::twoDWaterShader->setVec3("shallowWaterColor", glm::vec3(183.f / 255.f, 217.f / 255.f, 182.f / 255.f));
	Main::twoDWaterShader->setVec3("shallowWaterColor", glm::vec3(206.f / 255.f, 210.f / 255.f, 158.f / 255.f));
	Main::twoDWaterShader->setFloat("causticSize", 16.f);
}

void world::spawnFishSchool() {
	return;
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

void world::spawnFishTransporter() {
	fishTransporter = new AfishTransporter({ 1000.f, 650.f });
	makeDrawLists();
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

void world::draw(Shader* shaderProgram) {
	renderWater();

	// draw background
	//mapImg->draw(shaderProgram);
	
	//vault->draw(shaderProgram);
	if (Main::currWorld == "vault")
		return; // if clicked on vault
	// temp
	//if (merchantShop)
	//	merchantShop->draw(shaderProgram);

	for (int i = 0; i < fishSchoolList.size(); i++)
		fishSchoolList[i]->draw(shaderProgram);

	//*
	for (int i = 0; i < trees.size(); i++)
		trees[i]->draw(shaderProgram);
	//*/

	if (sellFish)
	sellFish->draw(shaderProgram);
	
	if (Main::pet)
		Main::pet->draw(shaderProgram);
	
	sortDraw(shaderProgram);

	/*
	if (rain) {
		rain->draw(shaderProgram);
		// makes screen darker
		SDL_SetRenderDrawColor(shaderProgram, 0, 0, 0, darkenValue);
		SDL_Rect rect = { 0, 0, stuff::screenSize.x, stuff::screenSize.y };
		SDL_RenderFillRect(shaderProgram, &rect);
	}
	*/

	Main::drawWidgets(shaderProgram);

	if (circleAnim && !circleAnim->finished())
		circleAnim->draw(shaderProgram);
}

void world::renderWater() {
	if (waterImg)
		waterImg->draw(Main::twoDWaterShader);

	glDisable(GL_DEPTH_TEST);

	// ==== DRAW 2D STUFF ====
	Main::twoDShader->Activate();

	// draws ship inbetween water and dock
	if (ship)
		ship->draw(Main::twoDShader);
	
	if (beachAnim)
		beachAnim->draw(Main::twoDShader);
	if (mapImg)
		mapImg->draw(Main::twoDShader);
}

void world::sortDraw(Shader* shaderProgram) {
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
		//Main::character->anim->setLoc({stuff::screenSize.x / 2 - (Main::character->anim->cellWidth / 2) * stuff::pixelSize, stuff::screenSize.y / 2 - Main::character->anim->cellHeight / 2 * stuff::pixelSize});
		//loc = math::screenToWorld(stuff::screenSize.x / 2 + Main::character->anim->cellWidth / 2 * stuff::pixelSize, stuff::screenSize.y / 2 + Main::character->anim->cellHeight / 2 * stuff::pixelSize);
	}

	//vector screenLoc = math::worldToScreen(loc);
	vector screenLoc = math::worldToScreen(SaveData::saveData.playerLoc, "topleft");

	std::vector<AautoFisher*> autoFishersBehind;
	std::vector<AautoFisher*> autoFishersInFront;
	std::vector<npc*> npcBehind;
	std::vector<npc*> npcInFront;
	std::vector<Abuilding*> buildingBehind;
	std::vector<Abuilding*> buildingInFront;

	// sort

	// calc autofisher
	for (int i = 0; i < autoFisherList.size(); i++) {
		if (autoFisherList[i]->inFrontPlayer)
			autoFishersInFront.push_back(autoFisherList[i]);
		else
			autoFishersBehind.push_back(autoFisherList[i]);
	}

	// calc npc
	//*
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
	//*/

	std::vector<Image*> dockPoleInFront;
	std::vector<Image*> dockPoleBehind;
	vector characterPos = Acharacter::getCharLoc();
	for (Image* dockPole : poleList) {
		// if pole is further down on screen than character && if x is within range, so I don't have problem with autoFisher string overlap
		vector poleLoc = dockPole->getLoc();
		if (poleLoc.y < characterPos.y && characterPos.x >= poleLoc.x - 10.f && characterPos.x <= poleLoc.x + 10.f)
			dockPoleInFront.push_back(dockPole);
		else
			dockPoleBehind.push_back(dockPole);
	}


	for (int i = 0; i < buildingBehind.size(); i++)
		buildingBehind[i]->draw(shaderProgram);

	// draw npcs behind
	for (int i = 0; i < npcBehind.size(); i++) {
		npcBehind[i]->draw(shaderProgram);
	}

	for (Image* dockPole : dockPoleBehind) {
		dockPole->draw(shaderProgram);
	}

	// draw autofishers behind
	for (int i = 0; i < autoFishersBehind.size(); i++) {
		autoFishersBehind[i]->draw(shaderProgram);
	}

	if (buyer && !buyer->inFrontPlayer)
		buyer->draw(shaderProgram);

	// if bobber above player, render behind
	if (Main::character->tempBobberLoc.y > Main::character->getCharScreenLoc().y) {
		Main::character->draw(shaderProgram);
		Main::character->drawFishingLine(shaderProgram);
	} else {
		Main::character->drawFishingLine(shaderProgram);
		Main::character->draw(shaderProgram);
	}

	if (buyer && buyer->inFrontPlayer)
		buyer->draw(shaderProgram);

	for (int i = 0; i < buildingInFront.size(); i++)
		buildingInFront[i]->draw(shaderProgram);

	// draw npcs infront
	for (int i = 0; i < npcInFront.size(); i++) {
		npcInFront[i]->draw(shaderProgram);
	}

	for (Image* dockPole : dockPoleInFront) {
		dockPole->draw(shaderProgram);
	}

	// draw autofishers infront
	for (int i = 0; i < autoFishersInFront.size(); i++) {
		autoFishersInFront[i]->draw(shaderProgram);
	}
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
	} else if (worldChangeLoc == worldLoc::zero) {
		SaveData::saveData.playerLoc = { 0, 0 };
	}
}

// world 1
world1::world1(int worldChangeLoc) {
	spawnLoc = { 517, 476 };
	houseLoc = { 1670, -870 };
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	//mapImg = new Image("./images/worlds/world1/map1 - Copy (3).png", { -500, -775 });
	mapImg = new Image("./images/map1.png", { 0, 0 }, true);
	//mapImg = new Image("./images/worlds/world1/newMap.png", { 0, 0 }, true);
	for (int i = 0; i < 19; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	ship = new Aship({ -164+500, 495-25 });

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png"};
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
	//circleAnim = new animation(.1, circleImgs, circle, false);

	setupAutoFishers();
	
	buyer = new buyAutoFisher({ 295, -170 });

	sellFish = new dumpster({ 739+110, 672+50 });

	// npcs
	sailor = new Asailor({ 10+500, -5+554 - 5 });
	fisherman = new Afisherman({ 143+8+500, 190+550 - 9 });
	atm = new Aatm({ 327 + 500-52, 186 + 547 - 9 + 12 });
	scuba = new Ascuba({ 354-105 + 500, -133 + 554-9 });
	petSeller = new ApetSeller({ 365+100 + 500, 255 + 554 - 9 });
	merchant = new Amerchant({ 143+410 + 500, 270 + 554 - 9 });
	mechanic = new Amechanic({ 560+60 + 500, 150 + 560 - 9 });

	if (SaveData::saveData.mechanicStruct[0].unlocked)
		fishTransporter = new AfishTransporter({ 417-140+500, 157+554 }); // { 406, -209 }

	// npc buildings
	//merchantShop = new Image("./images/merchantShop.png", { 570, -280 });
	house = new Ahouse({1157, 429+346 });
	merchantShop = new AmerchantShop({ 998, 368+405 });
	mechanicHouse = new AmechanicHouse({ 1130, 251+363 }); // { 1086 - 500, 285 - 500 }
	petShop = new ApetShop({ 930, 321+440 });

	// make trees
	std::vector<vector> treeLocs = { {624, 524}, {739, 514},{842, 502},{944, 485},{229, 480},{415, 477},{525, 477},{1101, 460},{663, 454},{310, 448},{114, 433},{475, 432},{773, 426},{574, 425},{1012, 425},{869, 422},{709, 409},{380, 406},{215, 402},{935, 385},{622, 384},{1155, 372},{803, 371},{1063, 368},{546, 367},{288, 349},{461, 338},{373, 332},{869, 330},{1000, 323},{231, 298},{331, 279},{922, 279},{988, 247},{910, 224} };
	std::vector<vector> bushLocs = { { 736, 434 }, { 166, 409 }, { 792, 392 }, { 528, 380 }, { 609, 369 }, { 408, 365 }, { 266, 332 }, { 955, 239 } };

	// temp
	for (int i = 0; i < treeLocs.size(); i++)
		treeLocs[i] += vector{500, 622};
	for (int i = 0; i < bushLocs.size(); i++)
		bushLocs[i] += vector{ 500, 531 };

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

	std::vector<vector> rockLocs = { { 1068, 699 }, { 1379, 689 } };
	for (int i = 0; i < rockLocs.size(); i++) {
		Arock* rock = new Arock(rockLocs[i]);
		buildingList.push_back(rock);
	}

	std::unordered_map<std::string, animDataStruct> beachData;
	beachData.insert({ "in", {{0, 0}, {14, 0}, .1, false } });
	beachData.insert({ "out", {{0, 1}, {14, 1}, .1, false } });
	beachAnim = new animation("water/beachSpriteSheet.png", 1152, 388, beachData, true, { 708, 511 });
	beachAnim->setAnimation("in");
	beachAnim->addFinishedCallback(&world::finishedBeachAnim);
	beachAnim->start();

	waterImg = new Image("./images/water/water.png", { 0, 0 }, true);

	// pole list
	std::vector<vector> poleLocs = { { 580, 478 },{ 622, 499 },{ 664, 520 }, { 706, 541 }, { 748, 562 }, { 790, 583 } };
	for (int i = 0; i < poleLocs.size(); i++) {
		Image* poleImg = new Image("./images/worlds/world1/dockPole.png", poleLocs[i], true);
		poleList.push_back(poleImg);
	}
}

void world::finishedBeachAnim() {
	if (beachAnim->currAnim == "in")
		beachAnim->setAnimation("out");
	else
		beachAnim->setAnimation("in");
	beachAnim->start();
}

void world::sortTreeList() {
	std::vector<Atree*> sortedList;
	int treeSize = trees.size();
	for (int j = 0; j < treeSize; j++) {
		Atree* curr = trees[0];
		trees.erase(trees.begin());
		bool added = false;
		for (int i = 0; i < sortedList.size(); i++) {
			if (curr->loc.y >= sortedList[i]->loc.y) {
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world2/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world3/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world4/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world5/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world6/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world7/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world7/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world8/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world9/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
	bankSellLoc = { 1000, 650 };
	setWorldChangeLoc(worldChangeLoc);

	for (int i = 0; i < 8; i++)
		mapAnimList.push_back("./images/worlds/world1/map" + std::to_string(i + 1) + ".png");
	//mapAnim = new animation(.1, mapAnimList, mapImg, true, { -500, -500 });
	//mapAnim->start();
	mapImg = new Image("./images/worlds/world10/map1.png", { -500, -500 }, true);

	std::string circlePath = "./images/circle/circle";
	circleImgs = std::vector<std::string>{ circlePath + "1.png", circlePath + "2.png", circlePath + "3.png", circlePath + "4.png" };
	circle = new Image(circlePath + "1.png", { 0, 0 }, false);
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
