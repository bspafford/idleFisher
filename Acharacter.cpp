#include "Acharacter.h"

#include <SDL_image.h>
#include <iostream>

#include "main.h"
#include "worlds.h"
#include "AautoFisher.h"
#include "sounds.h"
#include "collision.h"
#include "animation.h"
#include "timer.h"
#include "fishSchool.h"
#include "upgrades.h"
#include "petBuffs.h"
#include "baitBuffs.h"

// widget
#include "fishComboWidget.h"
#include "comboWidget.h"
#include "currencyWidget.h"
#include "fishUnlocked.h"
#include "heldFishWidget.h"
#include "premiumBuffWidget.h"
#include "UIWidget.h"
#include "comboOvertimeWidget.h"
#include "achievementWidget.h"
#include "newRecordWidget.h"

Acharacter::Acharacter() {
	std::unordered_map<std::string, animDataStruct> animData;
	// walking
	animData.insert({ "walkE", { {0, 0}, {5, 0}, .13, true } });
	animData.insert({ "walkNE", { {6, 0}, {11, 0}, .13, true } });
	animData.insert({ "walkN", { {12, 0}, {17, 0}, .13, true } });
	animData.insert({ "walkNW", { {18, 0}, {23, 0}, .13, true } });
	animData.insert({ "walkW", { {0, 1}, {5, 1}, .13, true } });
	animData.insert({ "walkSW", { {6, 1}, {11, 1}, .13, true} });
	animData.insert({ "walkS", { {12, 1}, {17, 1}, .13, true } });
	animData.insert({ "walkSE", { {18, 1}, {23, 1}, .13, true } });

	// idle
	animData.insert({ "idleE", { {0, 2}, {13, 2}, .13, true } });
	animData.insert({ "idleNE", { {14, 2}, {27, 2}, .13, true } });
	animData.insert({ "idleN", { {0, 3}, {13, 3}, .13, true } });
	animData.insert({ "idleNW", { {14, 3}, {27, 3}, .13, true } });
	animData.insert({ "idleW", { {0, 4}, {13, 4}, .13, true } });
	animData.insert({ "idleSW", { {14, 4}, {27, 4}, .13, true} });
	animData.insert({ "idleS", { {0, 5}, {13, 5}, .13, true } });
	animData.insert({ "idleSE", { {14, 5}, {27, 5}, .13, true } });

	// fishing
	animData.insert({ "castSE", { {0, 6}, {17, 6}, .1, false } });
	animData.insert({ "idleFishingSE", { {18, 6}, {31, 6}, .1, true } });
	animData.insert({ "transitionSE", { {0, 7}, {2, 7}, .1, false } });
	animData.insert({ "waitSE", { {3, 7}, {6, 7}, .1, true } });
	animData.insert({ "pullSE", { {7, 7}, {18, 7}, .1, false } });

	anim = new animation("character/characterSpriteSheet.png", 21, 49, animData, true, { 0, 0 });
	anim->setAnimation("idleSE", true);
	anim->addFinishedCallback(this, &Acharacter::animFinished);
	anim->addFrameCallback(this, &Acharacter::setFishingTipLoc);
	anim->start();

	std::unordered_map<std::string, animDataStruct> fishingRodData;
	fishingRodData.insert({ "castSE", { {0, 0}, {17, 0}, .1, false } });
	fishingRodData.insert({ "idleFishingSE", { {18, 0}, {31, 0}, .1, true } });
	fishingRodData.insert({ "transitionSE", { {32, 0}, {34, 0}, .1, false } });
	fishingRodData.insert({ "waitSE", { {35, 0}, {38, 0}, .1, true } });
	fishingRodData.insert({ "pullSE", { {39, 0}, {51, 0}, .1, false } });

	fishingRod = new animation("character/fishingRod.png", 108, 83, fishingRodData, true);

	fishingTimer = new timer();
	fishingTimer->addCallback(this, &Acharacter::fishing);
	bobberCatchTimer = new timer();
	bobberCatchTimer->addCallback(this, &Acharacter::bobberCatchAnim, &Acharacter::bobberCatchAnim);
	bobberBobTimer = new timer();
	bobberBobTimer->addCallback(this, &Acharacter::bobberBobAnim, &Acharacter::bobberBobAnim);

	bobberImg = std::make_unique<Fimg>("./images/bobber.png", vector{ 100, 100 });
	bobberWaterOverlay = std::make_unique<Fimg>("./images/bobberWaterOverlay.png", vector{ 0, 0 });

	std::unordered_map<std::string, animDataStruct> bobberData;
	bobberData.insert({ "water", {{0, 0}, {3, 0}, .3, true } });
	bobberWaterAnimFront = new animation("bobberWaterAnimFront.png", 15, 12, bobberData, false, { 0, 0 });
	bobberWaterAnimBack = new animation("bobberWaterAnimBack.png", 15, 12, bobberData, false, { 0, 0 });
	bobberWaterAnimFront->setAnimation("water");
	bobberWaterAnimBack->setAnimation("water");
	bobberWaterAnimFront->start();
	bobberWaterAnimBack->start();

	setPlayerColPoints();

	premiumCatchTimer = new timer();
	premiumCatchTimer->addCallback(this, &Acharacter::setCatchPremium);

	comboOvertimer = new timer();
	comboOvertimer->addCallback(this, &Acharacter::comboOvertimeFinished);
	if (Main::comboOvertimeWiget)
		Main::comboOvertimeWiget->addTimer(comboOvertimer);
}

void Acharacter::animFinished() {
	if (anim->currAnim.find("cast") != std::string::npos) { // if not cast
		// change to next animation
		anim->setAnimation("idleFishingSE", true);
		anim->start();
		fishingRod->setAnimation("idleFishingSE", true);
		fishingRod->start();
	} else if (anim->currAnim == "transitionSE") {
		anim->setAnimation("waitSE", true);
		anim->start();
		fishingRod->setAnimation("waitSE", true);
		fishingRod->start();
	} else if (anim->currAnim == "pullSE") {
		anim->setAnimation("castSE", true);
		anim->start();
		fishingRod->setAnimation("castSE", true);
		fishingRod->start();
	}
}

void Acharacter::setFishingTipLoc(int frame) {
	vector loc = vector{ Main::screenWidth / 2.f, Main::screenHeight / 2.f } + (vector{ -anim->cellWidth / 2.f, -anim->cellHeight / 2.f } + vector{ -46, -14 }) * stuff::pixelSize;

	if (anim->currAnim == "castSE") {
		std::vector<vector> castAnimLocs = std::vector<vector>{ {95, 41}, {95, 41}, {94, 41}, {94, 41}, {95, 42}, {80, 1}, {48, -1}, {21, 43}, {27, 55}, {30, 56}, {47, 0}, {105, 11}, {105, 11}, {100, 4}, {98, 10}, {98, 10}, {98, 10}, {98, 10}};
		fishingTipLoc = castAnimLocs[frame] * stuff::pixelSize + loc;
		if (frame >= 11)
			showFishingLine = true;
	} else if (anim->currAnim == "idleSE") {
		std::vector<vector> idleFishing = std::vector<vector>{ {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 10}, {98, 11}, {98, 11}, {98, 11}, {98, 11}, {98, 11} };
		fishingTipLoc = idleFishing[frame] * stuff::pixelSize + loc;
		showFishingLine = true;
	} else if (anim->currAnim == "transitionSE") {
		std::vector<vector> transition = std::vector<vector>{ {107, 14}, {105, 23}, {91, 27} };
		fishingTipLoc = transition[frame] * stuff::pixelSize + loc;
		showFishingLine = true;
	} else if (anim->currAnim == "waitSE") {
		std::vector<vector> pullAnim = std::vector<vector>{ {96, 33}, {90, 29}, {97, 35}, {89, 25} };
		fishingTipLoc = pullAnim[frame] * stuff::pixelSize + loc;
		showFishingLine = true;
	} else if (anim->currAnim == "pullSE") {
		std::vector<vector> catchAnim = std::vector<vector>{ {71 + 26, 33}, {71 + 26, 25}, {34 + 26, -19}, {3 + 26, -2}, {6 + 26, -3}, {6 + 26, -3}, {6 + 26, -3}, {3 + 26, -4}, {25 + 26, -6}, {62 + 26, 4}, {69 + 26, 41}, {31, -4} };
		fishingTipLoc = catchAnim[frame] * stuff::pixelSize + loc;
		if (frame >= 4)
			showFishingLine = false;
	}
}

void Acharacter::move(vector dir, float deltaTime) {
	moveDir = dir;

	if (fishingStop && dir.x == 0 && dir.y == 0)
		fishingStop = false;

	if (fishingStop)
		return;

	if (isFishing && (dir.x != 0 || dir.y != 0))
		stopFishing();

	if (!canMove || (dir.x == 0 && dir.y == 0))
		return;

	// need to normalize vector, so you don't move faster going diagnal
	vector normDir = math::normalize(moveDir);

	SaveData::saveData.playerLoc.x += normDir.x * speed * deltaTime;
	SaveData::saveData.playerLoc.y += normDir.y * speed * deltaTime;

	//std::cout << "moved: " << normDir.x * speed * deltaTime << ", " << normDir.y * speed * deltaTime;

	//setPlayerColPoints();
}

void Acharacter::setPlayerColPoints() {
	float radius = 5;
	int yOffset = 22 * stuff::pixelSize;
	//vector loc = math::screenToWorld({ Main::screenWidth / 2.f - radius, Main::screenHeight / 2.f - radius + yOffset }, false);
	vector loc = getCharLoc();

	if (col)
		delete col;
	col = new Fcollision(loc, radius, " ");
}

void Acharacter::draw(SDL_Renderer* renderer) {
	anim->setLoc(SaveData::saveData.playerLoc / stuff::pixelSize - vector{float(anim->cellWidth), float(anim->cellHeight)} / 2.f);
	anim->draw(renderer);
	// std::cout << anim->currAnim << ", " << anim->currFrameLoc << std::endl;
	if (isFishing) {
		//fishingRod->setLoc(SaveData::saveData.playerLoc / stuff::pixelSize - vector{ float(fishingRod->cellWidth), float(fishingRod->cellHeight) } / 2.f - vector{ 1.5, 17 });
		fishingRod->setLoc(anim->getLoc() - vector{45, 34.f});
		fishingRod->draw(renderer);
	}

	if (SaveData::saveData.fishData.size() > 0) {
		//std::cout << "dirtFish: ";
		//std::cout << "size: " << SaveData::saveData.fishData[1].numOwned.size() << ",   ";
		for (int i = 0; i < SaveData::saveData.fishData[1].numOwned.size(); i++) {
			//std::cout << SaveData::saveData.fishData[1].numOwned[i] << ", ";
		}

		//std::cout << std::endl;
	}


	
	// temp draw player col
	if (false) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		vector pos = col->points[0];
		float radius = col->radius;
		std::vector<vector> temp = { pos + radius, pos + vector{radius, -radius}, pos - radius, pos + vector{-radius, radius} };
		SDL_Point* asdf = new SDL_Point[int(temp.size()) + 1];
		for (int j = 0; j < temp.size(); j++) {
			vector screenPoint = math::worldToScreen({ float(temp[j].x), float(temp[j].y) });
			asdf[j] = { int(screenPoint.x), int(screenPoint.y) };
		}

		vector start = math::worldToScreen({ float(temp[0].x), float(temp[0].y) });
		asdf[temp.size()] = { int(start.x), int(start.y) };

		SDL_RenderDrawLines(Main::renderer, asdf, int(temp.size()) + 1);
	}
}

vector Acharacter::getCharLoc() {
	/*
	if (!isFishing)
		return math::screenToWorld(anim->getLoc() + vector{float(anim->cellWidth) / 2, float(anim->cellHeight)} *stuff::pixelSize);
	else
		return math::screenToWorld(anim->getLoc() + vector{float(anim->cellWidth) / 2, float(anim->cellHeight) - 19} *stuff::pixelSize);
		*/
	return anim->getLoc() - vector{ -float(anim->cellWidth / 2.f), -float(anim->cellHeight - 5)};
}

void Acharacter::Update(float deltaTime) {
	// animate character depending on move direction
	if (!isFishing) {
		std::vector<std::string> walkAnimList = { "walkNW", "walkN", "walkNE", "walkE", "walkSE", "walkS", "walkSW", "walkW" };
		std::vector<std::string> idleAnimList = { "idleNW", "idleN", "idleNE", "idleE", "idleSE", "idleS", "idleSW", "idleW" };

		if (moveDir.x != 0 || moveDir.y != 0) {
			prevMove = moveDir;
			float angle = atan2(moveDir.y, moveDir.x) * 180 / M_PI;
			int y = floor(1.f / 45.f * (angle + 45.f / 2.f)) + 3;
			if (anim->currAnim != walkAnimList[y])
				anim->setAnimation(walkAnimList[y], true);
		} else {
			float angle = atan2(prevMove.y, prevMove.x) * 180 / M_PI;
			int y = floor(1.f / 45.f * (angle + 45.f / 2.f)) + 3;
			if (anim->currAnim != idleAnimList[y])
				anim->setAnimation(idleAnimList[y], true);
		}
	}
}

void Acharacter::leftClick() {
	if (Main::currWidget)
		return;

	if (Main::hoverWater && SaveData::saveData.equippedFishingRod.id != -1 && !isFishing) {
		fishingStop = true;

		// puts bobber in water
			comboOvertimer->stop();
			Main::comboOvertimeWiget->setVisibility(false);

			isFishing = true;
			canMove = false;
			bobberLoc = Main::mousePos;
			tempBobberLoc = bobberLoc;

			bobberBobTimer->start(bobTime);

			// check if fishing rod is inside a fishSchool or not!
			calcFishSchoolUpgrades();

			// give upgrades
				// decrease fish time
				// increase catch num?

			catchTimer = math::randRange(upgrades::calcMinFishingInterval(), upgrades::calcMaxFishingInterval());
			// catchTimerGoing = true;
			fishingTimer->start(catchTimer);

			// face the character the correct direction
			// start fishing animation
			// temp
			vector fishRodPoint = { Main::screenWidth / 2, Main::screenHeight / 2};
			vector diff = math::normalize(bobberLoc - fishRodPoint);
			float angle = atan2(diff.y, diff.x) * 180 / M_PI;
			int y = floor(1.f / 45.f * (angle + 45.f / 2.f)) + 3.5;

			// set fishing anim
			//if (anim->animList != idleAnimWheel[y])
			showFishingLine = false;
			
			if (anim->currAnim.find("cast") == std::string::npos) { // if not cast animation, from any direction
				anim->setAnimation("castSE", true);
				anim->start();
				fishingRod->setAnimation("castSE", true);
				fishingRod->start();
			}
				//anim->setAnimation(idleAnimWheel[y], -1, true);

	// catch fish
	} else if (isFishing && Main::fishComboWidget->visible) {
		// catch fish
		if (upgrades::calcComboUnlocked()) {
			int combo = Main::fishComboWidget->click(Acharacter::isFishing);
			std::cout << "combo: " << upgrades::calcComboStart(upgrades::calcComboMax()) << std::endl;
			switch (combo) {
			case 0:
				if (!baitBuffs::chanceToKeepCombo()) // reset combo if false
					comboNum = upgrades::calcComboStart(upgrades::calcComboMax());
				break;
			case 2:
				if (comboUnlocked) {
					double comboMax = upgrades::calcComboMax();
					comboNum = math::clamp(comboNum + upgrades::calcComboIncrease(comboMax), 1, comboMax);
				}
				break;
			}

			Main::comboWidget->showComboText();
			Main::comboWidget->spawnComboNumber(comboNum);
		}

		Main::fishComboWidget->visible = false;

		// set animation
		anim->setAnimation("pullSE", true);
		anim->start();
		fishingRod->setAnimation("pullSE", true);
		fishingRod->start();
		// add fish
		if (currFish.id != 0) { // if not premium
			if (!SaveData::saveData.fishData[currFish.id].unlocked)
				Main::fishUnlocked->start(currFish);

			if (SaveData::saveData.fishData[currFish.id].biggestSizeCaught < currFishSize) {
				SaveData::saveData.fishData[currFish.id].biggestSizeCaught = currFishSize;
				if (SaveData::saveData.fishData[currFish.id].unlocked)
					Main::newRecordWidget->start(currFishSize);
			}

			double catchNum = upgrades::calcFishCatchNum();
			SaveData::saveData.fishData[currFish.id].unlocked = true;
			SaveData::saveData.fishData[currFish.id].numOwned[currFishQuality] += catchNum;
			SaveData::saveData.fishData[currFish.id].totalNumOwned[currFishQuality] += catchNum;

		} else { // if premium
			SaveData::saveData.currencyList[0].numOwned++;

			// give the player buffs
			// instance cash, instantly collect currency
			// low long buff, get more fish on catch
			// high fast buff, get more fish on catch
			premiumFishBuff();
			// start premium fish cooldown
			premiumCatchTimer->start(upgrades::calcPremiumCoolDownTime());
			canCatchPremium = false;

			Main::currencyWidget->updateList();
		}


		Main::checkAchievements();
		
		// updates held fish widget
		Main::heldFishWidget->updateList(Main::renderer);

		// check if fishing rod is inside a fishSchool or not!
		calcFishSchoolUpgrades();

		// start up fishing again
		catchTimer = math::randRange(upgrades::calcMinFishingInterval(), upgrades::calcMaxFishingInterval());
		// catchTimerGoing = true;
		fishingTimer->start(catchTimer);

		// set fish image
		fishImg = std::make_unique<Fimg>(currFish.thumbnail, bobberImg->loc);
		showFish = true;

		tempBobberLoc = bobberLoc;
		bobberBobTimer->stop();
		bobberCatchTimer->start(anim->animData[anim->currAnim].duration * 4);
	} else if (isFishing) { // stop fishing
		stopFishing();
	}
}

void Acharacter::fishing() {
	if (!isFishing || !canCatchWorldFish())
		return;

	// Show bobber combo
	currFish = calcFish(currFishQuality, currFishSize);

	Main::fishComboWidget->Start(currFish, currFishQuality);

	anim->setAnimation("transitionSE", true);
	anim->start();
	fishingRod->setAnimation("transitionSE", true);
	fishingRod->start();

	fishingTimer->stop();
}

FfishData Acharacter::calcFish(int &quality, float &fishSize) {
	float rand = math::randRange(0.f, 1.f);
	std::vector<std::vector<float>> probList = calcFishProbability(SaveData::data.fishData);

	for (int i = 0; i < probList.size(); i++)
		if (rand <= probList[i][1]) {
			float randQuality = math::randRange(0.f, 100.f);
			if (randQuality <= 1) // gold 1%
				quality = 3;
			else if (randQuality <= 6) // silver 5%
				quality = 2;
			else if (randQuality <= 18) // bronze 12%
				quality = 1;
			else
				quality = 0;

			FfishData caughtFish = SaveData::data.fishData[probList[i][0]];


			fishSize = math::randRangeInt(caughtFish.minSize, caughtFish.maxSize);
			std::cout << "fish size: " << fishSize << std::endl;
			return caughtFish;
		}

	// shouldn't hit this
	return SaveData::data.fishData[1];
}

// calculates the chance of each fish
// normalizes the value of the golden fish to always be its chance
// if its chance is .05 its always .05 even if another fish is 45% chance
// old method: premium = 1%, fish = 49%, actual premium chance = 2%, actual fish = 98%
// this method: premium = 1%, fish = 49%, actual premium chance = 1% actual fish = 99%
std::vector<std::vector<float>> Acharacter::calcFishProbability(std::vector<FfishData> fishData) {
	// get premium currency chance
	// then do (100 - premium chance) / 100
	// calc fish chance and multuply by that number
	// then add the premium chance at the end
	std::vector<float> petBuff = petBuffs::increaseChanceOfHigherFish();

	float premiumChance = canCatchPremium ? upgrades::calcPremiumCatchChance() : 0;
	float totalProb = 0; // premiumChance;
	for (int i = 1; i < fishData.size(); i++) {
		if (fishData[i].fishingPower <= SaveData::saveData.equippedFishingRod.power && (fishData[i].levelName == Main::currWorld || fishData[i].levelName == "premium")) {
			float val = fishData[i].probability;
			if (i < petBuff.size())
				val *= petBuff[i];
			totalProb += val;
		}
	}

	float multiplier = (1 - (premiumChance / 100));

	std::vector<std::vector<float>> probList;
	float test = 0;
	for (int i = 1; i < fishData.size(); i++) {
		if (fishData[i].fishingPower <= SaveData::saveData.equippedFishingRod.power && (fishData[i].levelName == Main::currWorld || fishData[i].levelName == "premium")) {
			float multi = 1;
			if (i < petBuff.size())
				multi = petBuff[i];
			test += fishData[i].probability / totalProb * multiplier * multi;
			probList.push_back(std::vector<float>{(float)fishData[i].id, test});
		}
	}

	if (canCatchPremium)
		probList.push_back({ std::vector<float>{(float)fishData[0].id, 1} });

	return probList;
}

void Acharacter::stopFishing() {

	setCanMove(true);

	// catchTimerGoing = false;
	fishingTimer->stop();

	Main::fishComboWidget->hitWallNum = 0;
	Main::fishComboWidget->fishLoc = 0;
	Main::fishComboWidget->visible = false;
	Main::fishComboWidget->fishMoveBack = false;
	
	isFishing = false;

	bobberBobTimer->stop();

	if (comboNum > 1) {
		comboOvertimer->start(1);
		Main::comboOvertimeWiget->setVisibility(true);
	} else
		Main::comboWidget->hideComboText();
}

void Acharacter::comboOvertimeFinished() {
	// remove widget
	Main::comboOvertimeWiget->setVisibility(false);
	comboNum = upgrades::calcComboStart(upgrades::calcComboMax());
	Main::comboWidget->hideComboText();
}

void Acharacter::premiumFishBuff() {
	// if get in green give more
	// if in fish school give more

	float rand = math::randRange(0.f, 1.f);

	std::cout << "rand: " << rand << ", ";

	if (rand <= .45) { // instant cash
		std::cout << "instant money!" << std::endl;
		// % from bank, or a certian amount of time, which ever is less
		int worldIndex = math::getWorldIndexFromName(Main::currWorld);

		// calc mps
		double mps = 0;
		for (AautoFisher* autoFisher : world::autoFisherList)
			mps += autoFisher->calcMPS();

		// calc 10% of currency + held fish
		double heldCurrency = SaveData::saveData.currencyList[worldIndex + 1].numOwned;
		for (FfishData fish : SaveData::data.fishData) {
			if (fish.levelName == Main::currWorld) {
				FsaveFishData* saveFish = &SaveData::saveData.fishData[fish.id];
				heldCurrency += saveFish->numOwned[0] /* temp */ * upgrades::getFishSellPrice(fish, 0);
			}
		}

		double min = math::min(mps * 900, heldCurrency * .15);
		double currency = round(min + 10); // + 10 for base value incase you have no held currency or autofishers
		SaveData::saveData.currencyList[worldIndex + 1].numOwned += currency;
		SaveData::saveData.currencyList[worldIndex + 1].totalNumOwned += currency;

		Main::currencyWidget->updateList();
	} else if (rand <= .9) { // low long buff
		std::cout << "10x for 1:30 min" << std::endl;
		UpremiumBuffWidget* buff = new UpremiumBuffWidget(SaveData::data.goldenFishData[2]);
		Main::premiumBuffList.push_back(buff);
		Main::UIWidget->setupLocs();
	} else { // high short buff
		std::cout << "100x for 10 sec" << std::endl;
		UpremiumBuffWidget* buff = new UpremiumBuffWidget(SaveData::data.goldenFishData[3]);
		Main::premiumBuffList.push_back(buff);
		Main::UIWidget->setupLocs();
	}
}

// tries to set canmove but will not work if something like a widget is active
void Acharacter::setCanMove(bool move) {
	if (move && Main::currWidget == NULL) {
		canMove = true;
	} else if (!move)
		canMove = false;
}

bool Acharacter::getCanMove() {
	return canMove;
}

// failed to catch
void Acharacter::comboExceeded() {
	comboNum = upgrades::calcComboStart(upgrades::calcComboMax());
	Main::comboWidget->showComboText();
	Main::comboWidget->spawnComboNumber(comboNum);

	Main::fishComboWidget->visible = false;
	catchTimer = math::randRange(upgrades::calcMinFishingInterval(), upgrades::calcMaxFishingInterval());
	fishingTimer->start(catchTimer);

	anim->setAnimation("pullSE");
	anim->start();
	fishingRod->setAnimation("pullSE");
	fishingRod->start();
}

// temp straight line to character
void Acharacter::bobberCatchAnim() {
	int pullFrames = 4;

	vector temp = { float(Main::screenWidth / 2), float(Main::screenHeight / 2 - anim->cellHeight / 2 * stuff::pixelSize) };

	float time = bobberCatchTimer->getTime();
	float timer = bobberCatchTimer->getMaxTime();

	float percent = floor(time / timer * pullFrames) / pullFrames;

	float x = math::lerp(tempBobberLoc.x, temp.x, percent);
	float y = math::lerp(tempBobberLoc.y, temp.y, percent);

	bobberLoc = { x, y };
	fishImg->loc = { x, y };

	if (bobberCatchTimer->finished()) {
		// plays pop sound when gets to charcter
		Mix_PlayChannel(-1, sounds::catchSound, 0);

		showFishingLine = false;
		bobberBobTimer->start(bobTime);
		showFish = false;
	}
}

void Acharacter::bobberBobAnim() {
	float time = bobberBobTimer->getTime();
	float timer = bobberBobTimer->getMaxTime();

	// if reeling in fish
	if (anim->currAnim.find("wait") != std::string::npos || anim->currAnim.find("pull") != std::string::npos) {
		float percent = time / timer;
		int num = floor(percent * 14);
		bobberLoc = tempBobberLoc + vector{ 0, num % 2 * stuff::pixelSize };

	} else {
		vector bobberY = { 0, round(sin(time / timer * 2 * M_PI)) * stuff::pixelSize };
		bobberLoc = tempBobberLoc + bobberY;
	}

	// restarts the timer
	if (bobberBobTimer->finished()) {
		tempBobberLoc = bobberLoc;
		bobberBobTimer->start(bobTime);
	}
}

AfishSchool* Acharacter::bobberInFishSchool() {
	for (AfishSchool* fishSchool : world::fishSchoolList) {
		// check if bobber is inside of fish school
		// if it is then return true
		// otherwise keep looking
		if (fishSchool->pointInSchool(math::screenToWorld(bobberLoc)));
			return fishSchool;
	}
	return nullptr;
}

void Acharacter::calcFishSchoolUpgrades() {
	if (currFishSchool)
		currFishSchool->removeFishNum();

	currFishSchool = bobberInFishSchool();
	fishTimeMultiplier = currFishSchool ? .75 : 1;
	fishSchoolMultiplier = currFishSchool ? 1.5 : 1;
}

bool Acharacter::canCatchWorldFish() {
	Main::currWorld;
	for (FfishData fish : SaveData::data.fishData) {
		if (Main::currWorld == fish.levelName) {
			if (SaveData::saveData.equippedFishingRod.power < fish.fishingPower)
				return false;
			return true;
		}
	}
	return false;
}

vector Acharacter::getCharScreenLoc() {
	int yOffset = 22 * stuff::pixelSize;
	return { Main::screenWidth / 2.f, Main::screenHeight / 2.f + yOffset };
}

void Acharacter::drawFishingLine(SDL_Renderer* renderer) {
	if (!isFishing || !showFishingLine)
		return;

	SDL_SetRenderDrawColor(renderer, 242, 233, 211, 255);

	vector start = fishingTipLoc;
	vector end = { Acharacter::bobberLoc.x + (Acharacter::bobberImg->w / 2 * stuff::pixelSize), Acharacter::bobberLoc.y + (Acharacter::bobberImg->h / 2 * stuff::pixelSize) };

	vector negative{ 1, 1 };
	vector diff = { round((end.x - start.x) / stuff::pixelSize), round((end.y - start.y) / stuff::pixelSize) };
	if (diff.x < 0) {
		negative.x = -1;
		diff.x *= -1;
	}
	if (diff.y < 0) {
		negative.y = -1;
		diff.y *= -1;
	}

	// offset
	vector p1 = diff;
	vector p2 = { 0, 0 };

	float minX = math::min(0, diff.x);
	float maxX = math::max(0, diff.x);
	float minY = math::min(0, diff.y);
	float maxY = math::max(0, diff.y);

	int sizeX = maxX - minX;
	int sizeY = maxY - minY;

	float prevY = 0;
	float diffY;
	float x, y, a;

	if (sizeX > 0 && sizeY > 0) {
		SDL_Rect* points = new SDL_Rect[sizeX];
		for (int i = 1; i < sizeX + 1; i++) {
			x = minX + i;

			// linear
			if (anim->currAnim.find("wait") != std::string::npos || anim->currAnim.find("pull") != std::string::npos) {
				float m = (p2.y - p1.y) / (p2.x - p1.x);
				float b = p1.y - m * p1.x;
				y = m * x + b;
			// parabola
			} else {
				a = (p2.y - p1.y) / ((-p1.x + p2.x) * (-p1.x + p2.x));
				y = a * ((x - p1.x) * (x - p1.x)) + p1.y;
			}

			int yInt = round(y * negative.y);

			diffY = yInt - prevY;
			if (diffY < 1)
				diffY = 1;

			points[i - 1] = SDL_Rect{ int(round(x * negative.x) * stuff::pixelSize + start.x), int((yInt - (int)diffY) * stuff::pixelSize + start.y), int(stuff::pixelSize), int(stuff::pixelSize * diffY) };

			prevY = yInt;
		}
		// draw a straight line
		SDL_RenderFillRects(renderer, points, sizeX);
		if (points) {
			delete[] points;
			points = nullptr;
		}
	} else {
		if (diff.x < 1)
			diff.x = 1;
		else if (diff.y < 1)
			diff.y = 1;

		vector loc;
		if (start.x > end.x)
			loc.x = end.x;
		else
			loc.x = start.x;

		if (start.y > end.y)
			loc.y = end.y;
		else
			loc.y = start.y;

		SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), int(stuff::pixelSize * diff.x), int(stuff::pixelSize * diff.y) }; // int(round(minX)), int(round(minY))
		SDL_RenderFillRect(renderer, rect);
		delete rect;
	}

	bobberImg->loc = bobberLoc;
	bobberWaterOverlay->loc = tempBobberLoc;
	bobberWaterAnimBack->setLoc(tempBobberLoc - vector{ 4, 2 } *stuff::pixelSize);
	bobberWaterAnimFront->setLoc(tempBobberLoc - vector{4, 2} * stuff::pixelSize);
	bobberWaterAnimBack->draw(renderer);
	bobberImg->draw(renderer, false);
	bobberWaterOverlay->draw(renderer, false);
	bobberWaterAnimFront->draw(renderer);

	if (showFish)
		fishImg->draw(renderer, false);
}

void Acharacter::setCatchPremium() {
	canCatchPremium = true;
}

void Acharacter::equipFishingRod(FfishingRodStruct* fishingRod) {
	SaveData::saveData.equippedFishingRod = *fishingRod;
	Main::achievementWidget->updateEquipmentWidget();
}

void Acharacter::equipBait(FbaitStruct* bait) {
	SaveData::saveData.equippedBait = *bait;
	Main::achievementWidget->updateEquipmentWidget();
}