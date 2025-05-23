#include "AautoFisher.h"

#include <SDL_image.h>
#include <iostream>

#include "main.h"
#include "worlds.h"
#include "stuff.h"
#include "animation.h"
#include "shortNumbers.h"
#include "collision.h"
#include "Acharacter.h"
#include "autoFisherUI.h"
#include "upgrades.h"

// widget
#include "heldFishWidget.h"
#include "currencyWidget.h"
#include "AFmoreInfoUI.h"
#include "autoFisherUI.h"
#include "button.h"
#include "text.h"

#include "textureManager.h"

AautoFisher::AautoFisher(int id) {
	if (!autoFisherSpriteSheet)
		autoFisherSpriteSheet = new Fimg("./images/autoFisher/autoFisher.png", { 0, 0 });
	if (!fishingLineSpriteSheet)
		fishingLineSpriteSheet = new Fimg("./images/autoFisher/fishingLine.png", { 0, 0 });
	if (!outlineSpriteSheet)
		outlineSpriteSheet = new Fimg("./images/autoFisher/outline.png", { 0, 0 });

	this->id = id;
	SaveData::saveData.autoFisherList[id].unlocked = true;
	loc = { SaveData::data.autoFisherData[id].xLoc, SaveData::data.autoFisherData[id].yLoc };
	level = &SaveData::saveData.autoFisherList[id].level;

	setupCollision();
	
	upgradeAnimIndex = *level / 10;

	// auto fisher animation
	std::unordered_map<std::string, animDataStruct> autoFisherData;
	autoFisherData.insert({ "1", {{0, 0}, {39, 0}, .1, true} });
	autoFisherData.insert({ "2", {{0, 1}, {39, 1}, .1, true} });
	autoFisherData.insert({ "3", {{0, 2}, {39, 2}, .1, true} });
	autoFisherData.insert({ "4", {{0, 3}, {39, 3}, .1, true} });
	autoFisherData.insert({ "5", {{0, 4}, {39, 4}, .1, true} });
	autoFisherData.insert({ "6", {{0, 5}, {39, 5}, .1, true} });
	autoFisherData.insert({ "7", {{0, 6}, {39, 6}, .1, true} });
	autoFisherData.insert({ "8", {{0, 7}, {39, 7}, .1, true} });
	autoFisherData.insert({ "9", {{0, 8}, {39, 8}, .1, true} });
	autoFisherData.insert({ "10", {{0, 9}, {39, 9}, .1, true} });
	autoFisherData.insert({ "11", {{0, 10}, {39, 10}, .1, true} });

	autoFisher = new animation(autoFisherSpriteSheet, 42, 58, autoFisherData, true, loc + vector{ -2, 5 });
	autoFisher->addAnimEvent(40, this, &AautoFisher::catchFish);
	autoFisher->setAnimation("1");
	autoFisher->start();

	// outline animation
	std::unordered_map<std::string, animDataStruct> outlineData;
	outlineData.insert({ "outline", {{0, 0}, {39, 0}, .1, true} });
	outline = new animation(outlineSpriteSheet, 42, 58, outlineData, true, loc + vector{ -2, 5 });
	outline->setAnimation("outline");
	outline->start();

	// fishing rod animation
	std::unordered_map<std::string, animDataStruct> fishingLineData;
	fishingLineData.insert({ "fishingLine", {{0, 0}, {39, 0}, .1, true} });
	fishingLine = new animation(fishingLineSpriteSheet, 138, 139, fishingLineData, true, loc + vector{ 11, -2 });
	fishingLine->setAnimation("fishingLine");
	fishingLine->start();

	setStats();

	autoFisherNum = world::autoFisherList.size();

	UI = new autoFisherUI(this, loc);

	afMoreInfoUI = new AFmoreInfoUI(this);
}

AautoFisher::~AautoFisher() {
	delete UI;
	delete afMoreInfoUI;
	delete autoFisher;
	delete outline;
	delete fishingLine;
}

void AautoFisher::Update(float deltaTime) {
	calcIfPlayerInfront();

	if (autoFisher)
		calcMouseOver(bMouseOver, mouseOver(autoFisher->spriteSheet));

	if (Main::bLeftClick)
		leftClick();

	if (Main::bRightClick)
		rightClick();
}

void AautoFisher::draw(SDL_Renderer* renderer) {
	if (UI && UI->visible)
		UI->draw(renderer);

	if (autoFisher)
		autoFisher->draw(renderer);
	if (fishingLine)
		fishingLine->draw(renderer);
	if (bMouseOver && outline)
		outline->draw(renderer);
}

void AautoFisher::leftClick() {
	if (bMouseOver) {
		collectFish();
		if (UI)
			UI->updateUI();
	}
}

void AautoFisher::rightClick() {
	// show auto fisher ui
	if (bMouseOver) {
		if (UI && !UI->visible)
			// close all other autofisher UIs
			for (int i = 0; i < world::autoFisherList.size(); i++) {
				if (world::autoFisherList[i] != this && world::autoFisherList[i]->UI->visible && !world::autoFisherList[i]->UI->closing)
					world::autoFisherList[i]->UI->closeUI();
				else if (!world::autoFisherList[i]->UI->closing)
					world::autoFisherList[i]->UI->visible = false;
			}
		if (UI)
			UI->openUI();
	}
}

// get mouse over
// if mouse is over that image border then do a better check to see what the pixel color is
// if alpha is 0 then mouse isn't over it
// needs screen coords
bool AautoFisher::mouseOver(Fimg* img) {
	vector min = math::worldToScreen(img->loc);
	vector max = min + (vector{float(img->w), float(img->h)} *stuff::pixelSize);

	if (min.x <= Main::mousePos.x && Main::mousePos.x <= max.x && min.y <= Main::mousePos.y && Main::mousePos.y <= max.y) {
		vector screenPos = min;
		vector pos = { Main::mousePos.x - screenPos.x, Main::mousePos.y - screenPos.y };
		SDL_Color pixelColor;
		pixelColor.a = 1;
		if (img->surface)
			pixelColor = math::GetPixelColor(img->surface, (int)pos.x, (int)pos.y);

		if ((int)pixelColor.a != 0)
			return true;
	}

	return false;
}

// calcs if mouse over and updates the mouse over num in main
// also updates the objects bool
// only updates is theres a change in the variables
void AautoFisher::calcMouseOver(bool& mouseOverPrev, bool mouseOver) {
	if (mouseOverPrev && !mouseOver) {
		Main::leaveHoverObject(NULL);
	} else if (!mouseOverPrev && mouseOver)
		Main::hoverObject(NULL);

	mouseOverPrev = mouseOver;
}

void AautoFisher::collectFish() {
	double currency = calcCurrencyHeld();
	for (int i = 0; i < heldFish.size(); i++) {
		SaveData::saveData.fishData[heldFish[i].id].numOwned[0] += heldFish[i].numOwned[0];
		SaveData::saveData.fishData[heldFish[i].id].totalNumOwned[0] += heldFish[i].numOwned[0];
		heldFish[i].numOwned[0] = 0;
	}

	Main::heldFishWidget->updateList(Main::renderer);

	// only start if full
	if (autoFisher && autoFisher->bStopped) {
		autoFisher->start();
		//outlineAnim->start();
	}

	if (afMoreInfoUI && afMoreInfoUI->isVisible())
		afMoreInfoUI->updateUI();
}

// go through all fish and make sure the auto fisher can catch them
// if cant then remove them from the probability list
void AautoFisher::catchFish() {
	FfishData currFish = calcFish();

	// temp
	int catchNum = 1;

	// temp
	// if there is enough room for the fish
	if (calcCurrencyHeld() + upgrades::getFishSellPrice(currFish, 0) * catchNum <= maxCurrency) {
		int index = calcCurrencyInList(currFish, heldFish);

		if (index != -1) {
			heldFish[index].id = currFish.id;
			heldFish[index].numOwned[0]++;
		} else {
			FsaveFishData saveFish;
			saveFish.id = currFish.id;
			// temp
			saveFish.numOwned.push_back(0);
			saveFish.numOwned.push_back(0);
			saveFish.numOwned.push_back(0);
			saveFish.numOwned.push_back(0);
			saveFish.numOwned[0] = catchNum;
			heldFish.push_back(saveFish);
		}

		if (autoFisher && calcCurrencyHeld() >= maxCurrency) {
			autoFisher->stop();
		}
	} else if (autoFisher) {
		autoFisher->stop();
	}

	if (afMoreInfoUI && afMoreInfoUI->isVisible())
		afMoreInfoUI->updateUI();
}

// returns -1 if currency isn't in list
int AautoFisher::calcCurrencyInList(FfishData fish, std::vector<FsaveFishData> heldFish) {
	for (int i = 0; i < heldFish.size(); i++) {
		if (heldFish[i].id == fish.id)
			return i;
	}

	return -1;
}

FfishData AautoFisher::calcFish() {
	float rand = math::randRange(0.f, 1.f);
	std::vector<std::vector<float>> probList = calcFishProbability(SaveData::data.fishData);

	for (int i = 0; i < probList.size(); i++) {
		if (rand <= probList[i][1]) {
			return SaveData::data.fishData[probList[i][0]];
		}
	}


	// shouldn't hit this
	return SaveData::data.fishData[1];
}

std::vector<std::vector<float>> AautoFisher::calcFishProbability(std::vector<FfishData> fishData, bool isCurrencyAFactor) {

	float totalProb = 0;// premiumChance;

	double heldCurrency = calcCurrencyHeld();

	// starts at 1 to skip premium
	for (int i = 1; i < fishData.size(); i++) {
		// see if autofisher has enough fishing power, see if theres enough room for the fish
		if (fishData[i].fishingPower <= fishingPower && (fishData[i].levelName == Main::currWorld || fishData[i].levelName == "premium")) {
			if ((isCurrencyAFactor && heldCurrency + upgrades::getFishSellPrice(fishData[i], 0) <= maxCurrency) || !isCurrencyAFactor)
				totalProb += float(fishData[i].probability);
		}
	}
	std::vector<std::vector<float>> probList;
	float test = 0;

	// starts at 1 to skip premium
	for (int i = 1; i < fishData.size(); i++) {
		if (fishData[i].fishingPower <= fishingPower && (fishData[i].levelName == Main::currWorld || fishData[i].levelName == "premium")) {
			if ((isCurrencyAFactor && heldCurrency + upgrades::getFishSellPrice(fishData[i], 0) <= maxCurrency) || !isCurrencyAFactor) {
				test += fishData[i].probability / totalProb;
					probList.push_back(std::vector<float>{(float)fishData[i].id, test});
			}
		}
	}

	return probList;
}

void AautoFisher::setupCollision() {
	// converts SDL_Point to vector
	std::vector<vector> points = std::vector<vector>(4);
	for (int i = 0; i < 4; i++)
		points[i] = { collisionPoints[i].x + loc.x, collisionPoints[i].y + loc.y };

	Fcollision* col = new Fcollision(points, " ");
	Main::allCollision.push_back(col);
}

void AautoFisher::calcIfPlayerInfront() {
	// should test if they are close enough to eachother?
	vector playerWorldLoc;
	if (Main::character->anim)
		playerWorldLoc = SaveData::saveData.playerLoc / stuff::pixelSize + Main::character->anim->cellHeight / 2;
	inFrontPlayer = playerWorldLoc.y < loc.y + autoFisher->cellHeight;
}

void AautoFisher::setStats() {
	// 1: .1, 100 = .01
	// updates the animation speed // temp values
	float y = -(1.f / 1100.f) * *level + 0.100909f;

	if (autoFisher)
		autoFisher->animData[autoFisher->currAnim].duration = y;
	outline->animData[outline->currAnim].duration = y;
	fishingLine->animData[fishingLine->currAnim].duration = y;

	// updates the max hold currency
	maxCurrency = *level * 100;
}

// takes input if the list isn't heldFish
double AautoFisher::calcCurrencyHeld(std::vector<FsaveFishData> fishList) {
	if (fishList.size() == 0)
		fishList = heldFish;

	double currency = 0;
	for (int i = 0; i < fishList.size(); i++) {
		FsaveFishData currSaveFish = fishList[i];
		FfishData currFish = SaveData::data.fishData[currSaveFish.id];
		currency += currSaveFish.numOwned[0] * upgrades::getFishSellPrice(currFish, 0);
	}

	return currency;
}

void AautoFisher::upgrade() {
	if (*level >= maxLevel)
		return;

	std::vector<double> levelPrice = getUpgradeCost();
	if (SaveData::saveData.currencyList[1].numOwned < levelPrice[1])
		return;

	// take currency
	SaveData::saveData.currencyList[1].numOwned -= levelPrice[1];
	*level += levelPrice[0];

	Main::currencyWidget->updateList();

	upgradeAnimIndex = floor(*level / 10);

	// update the autofisher animation
	setStats();

	// see if can fish again
	if (autoFisher && calcCurrencyHeld() < maxCurrency && autoFisher->bStopped)
		autoFisher->start();

	if (afMoreInfoUI && afMoreInfoUI->isVisible())
		afMoreInfoUI->updateUI();
}

// [0] for level, [1] for price
std::vector<double> AautoFisher::getUpgradeCost() {
	// temp
	double currency = SaveData::saveData.currencyList[1].numOwned;

	// int multiplier = multiplierList[multiplierIndex];

	int tempLevel = *level;
	if (multiplier != (int)INFINITY) {
		double cost = 0;
		for (int i = 0; i < multiplier; i++) {
			if (tempLevel + 1 > maxLevel)
				break;
			cost += price(tempLevel);
			tempLevel++;
		}

		// temp
		if (UI)
			UI->buttonText->setText(shortNumbers::convert2Short(cost));

		return { (double)tempLevel - *level, cost };
	} else {
		double minPrice = price(*level);
		// return 1 if don't have enough money for 1 upgrade
		if (currency < minPrice) {
			if (UI)
				UI->buttonText->setText(shortNumbers::convert2Short(minPrice));
			return { 1, minPrice };
		}

		double cost = 0;
		int num = 0;

		// if have enough money && isn't max level
		while (cost + price(num + *level) <= currency && *level + num < maxLevel) {
			//std::cout << "cost: " << cost << " + " << price(num + *level) << " <= " << currency << " && " << *level << " + " << num << " < " << maxLevel << std::endl;
			cost += price(num + *level);
			num++;
		}

		if (UI)
			UI->buttonText->setText(shortNumbers::convert2Short(cost));
		return { (double)num, cost };
	}
}

double AautoFisher::price(int level) {
	// 10 * 1.1^x
	return 10 * pow(1.1, level);
}

float AautoFisher::getCatchTime() {
	// temp
	// should calculate a max catch time, then set the fps based on catch time
	//return (float)NormalAnim[0].size() * autoFisherAnim->fps;
	if (autoFisher) {
		// std::cout << autoFisher->cellWidthNum << " * " << autoFisher->animData[autoFisher->currAnim].duration << std::endl;
		return autoFisher->cellWidthNum * autoFisher->animData[autoFisher->currAnim].duration;
	} else
		return 40; // temp
}

double AautoFisher::calcIdleProfits(double afkTime) {
	// needs to load how many fish it has in it before loading idle profits
	// temp
	/*
	FsaveFishData tempFish;
	tempFish.id = 1; // temp
	tempFish.numOwned = SaveData::saveData.autoFisherList[id].fullness;
	heldFish.push_back(tempFish);
	*/
	double currencyNum = 0;
	int numOfFishCatched = round(afkTime / getCatchTime());
	std::vector<vector> fishList = calcAutoFishList(numOfFishCatched);
	for (int i = 0; i < fishList.size(); i++) {
		//FfishData* currFish = &SaveData::data.fishData[i];
		FfishData* currFish = &SaveData::data.fishData[fishList[i].y];
		currencyNum += fishList[i].x * upgrades::getFishSellPrice(*currFish, 0);
	}

	return currencyNum;
}

// void AautoFisher::calcIdleProfits() {
/*
	// needs to load how many fish it has in it before loading idle profits
	// temp
	FsaveFishData tempFish;
	tempFish.id = 1; // temp
	tempFish.numOwned = SaveData::saveData.autoFisherList[id].fullness;
	heldFish.push_back(tempFish);

	double currencyHeld = calcCurrencyHeld();
	if (currencyHeld >= maxCurrency) { // don't load idle profits if already full
		autoFisherAnim->stop();
		return;
	}

	float timeDiffNano = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - SaveData::lastPlayed).count();

	// in seconds
	float timeDiff = timeDiffNano / 1000000000;

	// copy from UE5
	int numOfFishCatched = round(timeDiff / getCatchTime());

	std::vector<vector> fishList = calcAutoFishList(numOfFishCatched);

	std::vector<FsaveFishData> tempHeldFish;
	for (int i = 0; i < fishList.size(); i++) {
		FfishData* currFish = &SaveData::data.fishData[i];

		// adds the fish to the auto fisher save data, probably should do this in another script or function to make it look prettier
		if (currFish->levelName == Main::currWorld) {
			if (tempHeldFish.size() == 0) {
				// add fish
				FsaveFishData fishStats;
				fishStats.id = fishList[i].y;
				fishStats.numOwned = fishList[i].x;
				tempHeldFish.push_back(fishStats);

			} else {
				bool found = false;
				for (int j = 0; j < tempHeldFish.size(); j++) {
					// look for id of 0 // temp
					if (tempHeldFish[j].id == fishList[i].y) {
						tempHeldFish[j].numOwned += fishList[i].x;
						found = true;
						break;
					}
				}

				if (!found) {
					// add 0
					FsaveFishData fishStats;
					fishStats.id = fishList[i].y;
					fishStats.numOwned = fishList[i].x;
					tempHeldFish.push_back(fishStats);
				}
			}
		}
	}

	// clamps the fish gotten during idle to the max currency number
	float multi = 1;
	double tempCurrencyHeld = calcCurrencyHeld(tempHeldFish);
	double temp = maxCurrency - currencyHeld;
	if (tempCurrencyHeld + currencyHeld > temp) { // if theres no space in autoFisher
		multi = temp / tempCurrencyHeld;
		autoFisherAnim->stop();
	}

	for (int i = 0; i < tempHeldFish.size(); i++) {

		bool found = false;
		for (int j = 0; j < heldFish.size(); j++) {
			// look for id of 0 // temp
			if (tempHeldFish[i].id == heldFish[j].id) {
				// if found then
				heldFish[j].numOwned += round(multi * tempHeldFish[i].numOwned);
				found = true;
				break;
			}
		}

		if (!found) {
			FsaveFishData fishStats;
			fishStats.id = tempHeldFish[i].id;
			fishStats.numOwned = multi * tempHeldFish[i].numOwned;
			heldFish.push_back(fishStats);
		}
	}
}
*/

// { fish num, fish id }
std::vector<vector> AautoFisher::calcAutoFishList(int fishNum) {
	std::vector<FfishData> fishList;
	std::vector<vector> fishNumList;

	// temp
	int fishingRodId = 0; // saveData.currFishingRod.id;
	if (fishingRodId == -1)
		fishingRodId = 0;

	// calc all fish in world
	for (int i = 0; i < SaveData::saveData.fishData.size(); i++) {
		if (SaveData::data.fishData[i].levelName == Main::currWorld && fishingPower >= SaveData::data.fishData[i].fishingPower) {
			fishList.push_back(SaveData::data.fishData[i]);
		}
	}

	std::vector<float> fishChance = calcIdleFishChance(fishList);

	for (int i = 0; i < fishList.size(); i++) {
		int prob = fishChance[i] * (float)fishNum;
		vector fishIdList(prob, fishList[i].id);
		fishNumList.push_back(fishIdList);
	}

	return fishNumList;
}

// calculates what fish the auto fisher can catch
std::vector<float> AautoFisher::calcIdleFishChance(std::vector<FfishData> fishList) {
	std::vector<float> probList;
	float total = 0;

	for (int i = 0; i < fishList.size(); i++) {
		total += fishList[i].probability;
	}

	for (int i = 0; i < fishList.size(); i++) {
		probList.push_back(fishList[i].probability / total);
	}

	return probList;
}

void AautoFisher::startFishing() {
	if (autoFisher && calcCurrencyHeld() < maxCurrency && autoFisher->bStopped)
		autoFisher->start();
}

// fish per second
double AautoFisher::calcFPS() {
	// how fast it can catch fish
	return 1 / getCatchTime();
}

// money per second
double AautoFisher::calcMPS() {
	// fps
	// avg currency
	// avg fish cought

	std::vector<std::vector<float>> probList = calcFishProbability(SaveData::data.fishData, false);

	// total
	// price * percent

	float prevPercent = 0;
	float totalPrice = 0;
	for (int i = 0; i < probList.size(); i++) {
		// * %
		int id = probList[i][0];
		float percent = probList[i][1];

		totalPrice += (percent - prevPercent) * upgrades::getFishSellPrice(SaveData::data.fishData[id], 0);
		//std::cout << "percent: " << percent << ", price: " << ((percent - prevPercent) * upgrades::getFishSellPrice(SaveData::data.fishData[id])) << std::endl;

		prevPercent = percent;
	}

	return totalPrice * calcFPS();
}