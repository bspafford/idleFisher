/*

fix link error by having (int argc, char* argv[]) as parameters in main
fix no SDL2.dll by putting the dll from the lib x64 folder into where the .exe is for the project

*/

#include "main.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>
#include <SDL_mixer.h>

#include "csvReader.h"
#include "sounds.h"
#include "stuff.h"
#include "timer.h"
#include "fps.h"
#include "textureManager.h"
#include "Acharacter.h"
#include "camera.h"
#include "AautoFisher.h"
#include "collision.h"
#include "AStar.h"
#include "upgrades.h"
#include "achievement.h"
#include "achievementBuffs.h"

// npc
#include "fishTransporter.h"
#include "pet.h"
#include "atm.h"

// widgets
#include "widget.h"
#include "pauseMenu.h"
#include "settings.h"
#include "fishComboWidget.h"
#include "heldFishWidget.h"
#include "currencyWidget.h"
#include "comboWidget.h"
#include "achievementWidget.h"
#include "journal.h"
#include "fishUnlocked.h"
#include "UIWidget.h"
#include "premiumBuffWidget.h"
#include "idleProfitWidget.h"
#include "comboOvertimeWidget.h"
#include "newRecordWidget.h"

// worlds
#include "worlds.h"

int main(int argc, char* argv[]) {
	srand(time(0));
	math::randRange(0, 100); // steups up srand i guess? otherwise first value always the same

	TTF_Init();

	Main* _main = new Main();
	_main->createWindow();
	return 0;
}

void Main::Start(SDL_Renderer* renderer) {
	textureManager::textureManager();

	// setup sounds
	sounds();

	// makes opacity work
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	csvReader();
	SaveData::load();

	upgrades::init();
	achievementBuffs::init();

	setupWidgets(renderer);
	openLevel(SaveData::saveData.currWorld, worldLoc::None, true);

	// load character
	camera = new Ucamera();
	character = new Acharacter();
	if (SaveData::saveData.equippedPet.id != -1)
		Main::pet = new Apet(&SaveData::saveData.equippedPet, { 400, -200 });


	//world::buyer->updateLoc();

	// load idle profits
	loadIdleProfits();
	if (world::autoFisherList.size() > 0 /* && have fish transporter */) {
		idleProfitWidget->addToViewport(true);
	}

	// load widgets

	// load images
	setupImages(renderer);

	// setup collision objects
	//collision::getCollisionObjects(allCollision);
	// collision::getCollisionObjects(allCollision, collisionIdentifier, collisionWorld);

	// initiate keys list
	for (int i = 0; i < 322; i++) {
		KEYS[i] = false;
	}

	fps::fps();

	achievements = achievement::createAchievementList();
}

void Main::Update(float deltaTime) {
	/*
	if (1 / deltaTime < 1000)
		std::cout << "low fps: " << 1 / deltaTime << std::endl;
	//fps::update(deltaTime);
	//fps::showFPS(true);
	//float fps = round(1 / deltaTime) * timeScale;
	//std::cout << fps << std::endl; // output fps
	//*/

	// calculates outlier fps
	//fps::outliers(deltaTime);

	calcRebirthCurrency();
	
	// check if key down them move character
	move = { 0, 0 };
	if (KEYS[SDLK_w])
		move = { move.x, -1 };
	if (KEYS[SDLK_s])
		move = { move.x, 1 };
	if (KEYS[SDLK_a])
		move = { -1, move.y };
	if (KEYS[SDLK_d]) {
		move = { 1, move.y };
	}

	character->move(move, deltaTime);
	collision::testCollisions(character->col, allCollision, deltaTime);
	
	character->setPlayerColPoints();

	camera->Update(deltaTime);

	calcMouseImg();

	timer::callUpdate(deltaTime);
	character->Update(deltaTime);
	for (int i = 0; i < world::autoFisherList.size(); i++)
		world::autoFisherList[i]->Update(deltaTime);
	fishComboWidget->Update(deltaTime);

	// fade from black timer
	if (fadeFromBlack) {
		blackFadeTime += deltaTime;
		blackFadeY = 255 - pow(blackFadeTime / blackFadeTimer, 1) * 255;
		if (blackFadeTime >= blackFadeTimer) {
			fadeFromBlack = false;
			blackFadeTime = 0;
			blackFadeY = 0;
		}
	}

	if (world::fishTransporter)
		world::fishTransporter->update(deltaTime);
	if (pet)
		pet->update(deltaTime);

	//std::cout << "pos: " << SaveData::saveData.playerLoc << ",      ";

	// output mouse pos
	// vector mouseWorldPos = math::screenToWorld(mousePos);
	// std::cout << mouseWorldPos.x << ", " << mouseWorldPos.y << std::endl;
	// std::cout << mousePos << std::endl;
}

void Main::createWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Idle Fisher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // -1 auto selects driver, SDL_RENDERER_ACCELERATED makes it use hardware acceleration


	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_Surface* icon = IMG_Load("./images/icon.png");
	SDL_SetWindowIcon(window, icon);

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // set window fullscreen

	SDL_ShowCursor(false);

	running = true;

	// fps timer
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point currentTime;

	Start(renderer);
	//draw(renderer);

	SDL_Event e;
	while (running) {
		//Sleep(1000.f/10.f); // caps fps

		// fps stuff
		currentTime = std::chrono::steady_clock::now();
		float timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		
		float deltaTime = timeDiff / 1000000000 * timeScale;
		lastTime = currentTime;

		// run every frame
		Update(deltaTime);

		bLeftClick = false;
		bRightClick = false;
		mouseWheelDir = 0;

		// user input
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				// stops key repeat
				if (e.key.keysym.sym <= sizeof(KEYS) && KEYS[e.key.keysym.sym])
					break;

				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && currWorld != "titleScreen") {
					if (currWidget) {
						// if curr widget has parent then it opens up previous widget
						if (currWidget->parent)
							currWidget->parent->addToViewport(true);
						else
							currWidget->removeFromViewport();
					} else
						pauseMenu->addToViewport(true);
				}

				// temp
				if (e.key.keysym.scancode == SDL_SCANCODE_P)
					camera->detachCamera();
				
				// temp
				if (e.key.keysym.scancode == SDL_SCANCODE_K)
					SaveData::save();
				// temp
				if (e.key.keysym.scancode == SDL_SCANCODE_J) {
					SaveData::saveData.currencyList[1].numOwned += 9000;
					SaveData::saveData.currencyList[1].totalNumOwned += 9000;
					SaveData::saveData.currencyList[2].numOwned += 150;

					for (int i = 0; i < 0; i++) {
						SaveData::saveData.currencyList[i + 1].numOwned += 1000;
						SaveData::saveData.currencyList[i + 1].totalNumOwned += 1000;
						SaveData::saveData.currencyList[i + 1].unlocked = true;

						//if (i < 5)
						//	SaveData::saveData.fishData[i+1].unlocked = true;
					}
					currencyWidget->updateList();
				}


				if (e.key.keysym.sym <= sizeof(KEYS))
					KEYS[e.key.keysym.sym] = true;

				// temp
				if (e.key.keysym.scancode == SDL_SCANCODE_C)
					achievementWidget->addToViewport(true);
				if (e.key.keysym.scancode == SDL_SCANCODE_V)
					journal->addToViewport(true);

				// temp switch worlds to rebirth
				if (e.key.keysym.scancode == SDL_SCANCODE_O)
					openLevel("rebirth", worldLoc::changeWorlds, false);

				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT) {
					character->leftClick();
					bLeftMouseButtonDown = true;
				}
				if (e.button.button == SDL_BUTTON_RIGHT) {
					character->stopFishing();
					bRightMouseButtonDown = true;
				}
				
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT) {
					bLeftClick = true;
					bLeftMouseButtonDown = false;
				}
				if (e.button.button == SDL_BUTTON_RIGHT) {
					bRightClick = true;
					bRightMouseButtonDown = false;
				}

				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym < sizeof(KEYS))
					KEYS[e.key.keysym.sym] = false;
				break;
			case SDL_MOUSEMOTION:
				mousePos = { (float)e.motion.x, (float)e.motion.y };
				break;
			case SDL_MOUSEWHEEL:
				mouseWheelDir = e.wheel.y;
				break;
			case SDL_WINDOWEVENT:
				// on window resize
				if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					screenWidth = e.window.data1;
					screenHeight = e.window.data2;

					widget::resizeScreen();
				}
				break;
			default:
				break;
			}
		}
		// draw screen
		draw(renderer);
	}

	SDL_Quit();
}

void Main::calcMouseImg() {
	// if widget over screen, then don't test mouse collision for water
	if (currWidget) {
		if (hoverNum == 0 && cursorImg->path != "./images/cursor.png")
			cursorImg->setImage("./images/cursor.png");
		else if (hoverNum != 0 && cursorImg->path != "./images/cursor1.png")
			cursorImg->setImage("./images/cursor1.png");

		return;
	}

	if (hoverNum != 0 && cursorImg->path != "./images/cursor1.png") {
		hoverWater = false;
		cursorImg->setImage("./images/cursor1.png");
	} else if (mouseOverWater && hoverNum == 0 && cursorImg->path != "./images/hook.png") {
		hoverWater = true;
		cursorImg->setImage("./images/hook.png");
	} else if (hoverNum == 0 && !mouseOverWater && cursorImg->path != "./images/cursor.png") {
		hoverWater = false;
		cursorImg->setImage("./images/cursor.png");
	}
}

void Main::setupWidgets(SDL_Renderer* renderer) {
	pauseMenu = new UpauseMenu();
	settingsWidget = new Usettings();

	fishComboWidget = new UfishComboWidget(renderer);

	heldFishWidget = new UheldFishWidget();
	heldFishWidget->updateList(renderer);
	currencyWidget = new UcurrencyWidget();
	currencyWidget->updateList();

	comboWidget = new UcomboWidget();
	achievementWidget = new UachievementWidget();
	journal = new Ujournal();
	fishUnlocked = new UfishUnlocked();

	UIWidget = new UUIWidget();

	idleProfitWidget = new UidleProfitWidget();

	comboOvertimeWiget = new UcomboOvertimeWidget();

	newRecordWidget = new UnewRecordWidget();
}

void Main::setupImages(SDL_Renderer* renderer) {
	// cursor img
	cursorImg = std::make_unique<Fimg>("./images/cursor.png");
}

void Main::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 104, 153, 185, 255); // background color // 92, 127, 153, 255
	SDL_RenderClear(renderer); // clears screen

	leftClickList.clear();

	if (currWorld == "titleScreen")
		titleScreen::draw(renderer);
	else if (currWorld == "vault") {
		vaultWorld::draw(renderer);
	} else if (currWorld == "rebirth") {
		rebirthWorld::draw(renderer);
	} else if (currWorld == "world1") {
		world1::draw(renderer);
	} else if (currWorld == "world2")
		world2::draw(renderer);
	else if (currWorld == "world3")
		world3::draw(renderer);
	else if (currWorld == "world4")
		world4::draw(renderer);
	else if (currWorld == "world5")
		world5::draw(renderer);
	else if (currWorld == "world6")
		world6::draw(renderer);
	else if (currWorld == "world7")
		world7::draw(renderer);
	else if (currWorld == "world8")
		world8::draw(renderer);
	else if (currWorld == "world9")
		world9::draw(renderer);
	else if (currWorld == "world10")
		world10::draw(renderer);

	// draw A* pathfinding grid
	//Astar::drawBoard(renderer);
	// temp

	if (currWidget)
		currWidget->draw(renderer);

	if (fadeFromBlack) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackFadeY);
		SDL_Rect rect{ 0, 0, screenWidth, screenHeight };
		SDL_RenderFillRect(renderer, &rect);
	}

	fishUnlocked->draw(renderer);

	// draw collision
	// collision::showCollisionBoxes(renderer, allCollision);
	// draw mouse // draw last

	cursorImg->loc = mousePos;
	cursorImg->draw(renderer, false);

	// call last function in list
	if (leftClickList.size() > 0)
		leftClickList[leftClickList.size() - 1]();

	SDL_RenderPresent(renderer); // draws the renderer to the screen
}

void Main::drawWidgets(SDL_Renderer* renderer) {
	if (currWidget == pauseMenu)
		return;

	fishComboWidget->draw(renderer, screenWidth, screenHeight);
	heldFishWidget->draw(renderer);
	//currencyWidget->draw(renderer);
	comboWidget->draw(renderer);
	//UIWidget->draw(renderer);
	comboOvertimeWiget->draw(renderer);
	newRecordWidget->draw(renderer);
}

void Main::rebirth() {
	/*
	std::cout << "restarting everything" << std::endl;
	// resest all progress
	// gives rebirth currency and total rebirth currency
	double addedRebirth = calcRebirthCurrency(); // temp
	double rebirthCurrency = SaveData::saveData.rebirthCurrency + addedRebirth;
	double totalRebirthCurrency = SaveData::saveData.totalRebirthCurrency + addedRebirth;
	std::vector<FsaveRebirthStruct> rebirthData = SaveData::saveData.rebirthList;
	SaveData::saveData = FsaveData(); // clears all the data
	SaveData::saveData.rebirthCurrency = rebirthCurrency;
	SaveData::saveData.totalRebirthCurrency = totalRebirthCurrency;
	SaveData::saveData.rebirthList = rebirthData;
	SaveData::recalcLists();

	// update everything
		// UIs
			// currency
			// held fish
		// autofishers
		// 

	currWidget->removeFromViewport();
	world::autoFisherList.clear();
	heldFishWidget->updateList(renderer);
	currencyWidget->updateList();
	openLevel("world1", 1, true);
	*/

	// resest all progress
	// gives rebirth currency and total rebirth currency
	double addedRebirth = calcRebirthCurrency(); // temp
	SaveData::saveData.rebirthCurrency += addedRebirth;
	SaveData::saveData.totalRebirthCurrency += addedRebirth;

	// reset stuff
	vector playerLoc = { 200, -84 };

	//SaveData::saveData.fishData 
	for (int i = 0; i < SaveData::saveData.fishData.size(); i++) {
		FsaveFishData* currFish = &SaveData::saveData.fishData[i];
		currFish->numOwned = std::vector<double>(4);
	}

	SaveData::saveData.currencyList = std::vector<FsaveCurrencyStruct>(0);
	SaveData::saveData.upgradeList = std::vector<FsaveUpgradeStruct>(0);
	SaveData::saveData.worldList = std::vector<FsaveWorldStruct>(0);
	SaveData::saveData.mechanicStruct = std::vector<FsaveMechanicStruct>(0);
	SaveData::saveData.autoFisherList = std::vector<FsaveAutoFisherStruct>(0);
	SaveData::saveData.petList = std::vector<FsavePetStruct>(0);
    SaveData::saveData.equippedPet.id = -1;
	SaveData::saveData.fishingRodList = std::vector<FsaveFishingRodStruct>(0);
	SaveData::saveData.equippedFishingRod.id = -1;
	SaveData::saveData.baitList = std::vector<FsaveBaitStruct>(0);
	SaveData::saveData.buffList = std::vector<FsaveBuffStruct>(0);

	pet = NULL;

	SaveData::recalcLists();

	currWidget->removeFromViewport();
	world::autoFisherList.clear();
	heldFishWidget->updateList(renderer);
	currencyWidget->updateList();
	openLevel("world1", 1, true);
}

double Main::calcRebirthCurrency() {
	double totalPoints = 0; // total amount of points, calculated based on fish
	for (int i = 0; i < SaveData::saveData.currencyList.size(); i++) {
		FsaveCurrencyStruct* curr = &SaveData::saveData.currencyList[i];
		totalPoints += curr->totalNumOwned * curr->id;
	}

	double n = pow(SaveData::saveData.totalRebirthCurrency, double(3)) * double(10);			// how many points needed for each level (level 1 = 10, level 2 = 80...)
	double tempRebirthCurrency = std::cbrt((totalPoints + n) / double(10));						// returns level with input of currency
	double pointsToNextRebirth = pow(floor(tempRebirthCurrency + double(1)), double(3)) * double(10); // points needed to get to next level
	double lastRebirthNum = pow(floor(tempRebirthCurrency), double(3)) * double(10);		// how many points the last level took so pretty much y = f(x-1)

	double progress = (totalPoints + n - lastRebirthNum) / (pointsToNextRebirth - lastRebirthNum);
	UIWidget->updateProgressBar(tempRebirthCurrency - SaveData::saveData.totalRebirthCurrency, progress, 0);

	return floor(tempRebirthCurrency - SaveData::saveData.totalRebirthCurrency);
}

// only need to make this fishing line list once at the start of when the bobbers placed, then reupdate it for the animation?
// very unoptimized

void Main::loadIdleProfits() {
	float timeDiffNano = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - SaveData::lastPlayed).count();
	// in seconds
	float timeDiff = timeDiffNano / 1000000000;
	std::cout << "timeDiff: " << timeDiff << std::endl;

	if (world::fishTransporter)
		world::fishTransporter->calcIdleProfits(timeDiff);
	if (world::atm)
		world::atm->calcIdleProfits(timeDiff);

	/*
	// load auto fishers
	for (int i = 0; i < world::autoFisherList.size(); i++)
		if (world::autoFisherList[i])
			world::autoFisherList[i]->calcIdleProfits();
			*/
}

void Main::openLevel(std::string worldName, int worldChangeLoc, bool overrideIfInWorld) {
	// returns if the world is already open
	if (!overrideIfInWorld && worldName == currWorld)
		return;

	if (worldName == "vault" && currWorld != "vault")
		prevWorld = currWorld;
	currWorld = worldName;
	hoverNum = 0;

	if (currWidget)
		currWidget->removeFromViewport();

	collision::getCollisionObjects(allCollision);

	// deconstruct worlds
	titleScreen::deconstructor();
	world::deconstructor();
	vaultWorld::deconstructor();
	rebirthWorld::deconstructor();

	// temp
	if (currWorld == "titleScreen") {
		titleScreen::titleScreen();
		titleScreen::start();
	} else if (currWorld == "vault") {
		vaultWorld::vaultWorld();
		vaultWorld::start();
	} else if (currWorld == "rebirth") {
		rebirthWorld::rebirthWorld();
		rebirthWorld::start();
	} else if (currWorld == "world1") {
		world1::world1(worldChangeLoc);
		world1::start();
	} else if (currWorld == "world2") {
		world2::world2(worldChangeLoc);
		world2::start();
	} else if (currWorld == "world3") {
		world3::world3(worldChangeLoc);
		world3::start();
	} else if (currWorld == "world4") {
		world4::world4(worldChangeLoc);
		world4::start();
	} else if (currWorld == "world5") {
		world5::world5(worldChangeLoc);
		world5::start();
	} else if (currWorld == "world6") {
		world6::world6(worldChangeLoc);
		world6::start();
	} else if (currWorld == "world7") {
		world7::world7(worldChangeLoc);
		world7::start();
	} else if (currWorld == "world8") {
		world8::world8(worldChangeLoc);
		world8::start();
	} else if (currWorld == "world9") {
		world9::world9(worldChangeLoc);
		world9::start();
	} else if (currWorld == "world10") {
		world10::world10(worldChangeLoc);
		world10::start();
	}

	AStar::init();
}

void Main::checkAchievements() {
	for (auto& achievement : achievements) {
		achievement->checkUnlock();
	}
}

// if object then doesn't work with ui
// if not object then works with ui
void Main::hoverObject(widget* widget1) {
	// if (currWidget && hoverobject is apart of curr object)

	// could does hoverObject(widget* widget)
	// then in each of hte button instantiators just have it specify the widget, can be NULL, if not on widget
	// then check to see if currWidget and button->widget are equal
	// if they are then hover
	// else don't

	if ((widget1 && widget1 == currWidget) || (!widget1 && !currWidget)) {
		if (hoverNum == 0) {
			hoverWater = false;
		}
		// then hover
		hoverNum++;
	}
}

void Main::leaveHoverObject(widget* widget) {
	if ((widget && widget == currWidget) || (!widget && !currWidget))
		hoverNum--;

	if (hoverNum < 0)
		hoverNum = 0;
}

void Main::addLeftClick(void (*callback) ()) {
	leftClickList.push_back(callback);
}

void Main::addLeftClick(std::function<void()> callback) {
	leftClickList.push_back(callback);
}

vector Main::getScreenSize() {
	return { float(Main::screenWidth), float(Main::screenHeight) };
}