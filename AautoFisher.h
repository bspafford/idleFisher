#pragma once

#include "math.h"
#include "saveData.h"

#include <string>
#include <vector>

class autoFisherUI;
class AFmoreInfoUI;

class Ubutton;
class animation;

class AautoFisher {
public:
	AautoFisher(int id);
	~AautoFisher();

	void Update(float deltaTime);
	void draw(SDL_Renderer* renderer);

	void leftClick();
	void rightClick();
	bool mouseOver(Fimg* img);
	void collectFish();
	void catchFish();
	FfishData calcFish();
	std::vector<std::vector<float>> calcFishProbability(std::vector<FfishData> fishData, bool isCurrencyAFactor = true);
	int calcCurrencyInList(FfishData fish, std::vector<FsaveFishData> heldFish);
	std::vector<double> getUpgradeCost();
	double price(int level);
	double calcCurrencyHeld(std::vector<FsaveFishData> fishList = std::vector<FsaveFishData>(0));
	// gets the autofisher to start fishing again once it is no longer full
	void startFishing();

	void calcMouseOver(bool &mouseOverPrev, bool mouseOver);

	void upgrade();

	float getCatchTime();

	double calcIdleProfits(double afkTime);
	std::vector<vector> calcAutoFishList(int fishNum);
	std::vector<float> calcIdleFishChance(std::vector<FfishData> fishList);

	double calcFPS();
	double calcMPS();

	// used to display the correct color of auto fisher
	int upgradeAnimIndex;

	bool bMouseOver = false;

	animation* autoFisher;
	animation* outline;
	animation* fishingLine;

	vector loc;

	// collision
	void setupCollision();
	SDL_Point* collisionPoints = new SDL_Point[4]{ {1, 59}, { 17, 51 }, { 29, 56 }, { 11, 64 } };
	// std::vector<vector> collisionPoints = std::vector<vector>{ {1, 59}, { 17, 51 }, { 29, 56 }, { 11, 64 } };

	void calcIfPlayerInfront();
	bool inFrontPlayer = false;

	// world
	int* level = new int(1);
	int maxLevel = 100;
	int maxCurrency = 100;

	// temp
	std::vector<FsaveFishData> heldFish = std::vector<FsaveFishData>();

	autoFisherUI* UI;

	// temp
	float fishingPower = 50;

	// multiplier stuff
	// std::vector<int> multiplierList{ 1, 10, (int)INFINITY };
	int multiplier = 1;

	// temp
	int currencyId = 1;

	int autoFisherNum = 0;

	AFmoreInfoUI* afMoreInfoUI;

	int id = -1;

	// sets the stats (fullness, and speed) based on auto fisher level after load
	void setStats();

private:
	static inline Fimg* autoFisherSpriteSheet;
	static inline Fimg* fishingLineSpriteSheet;
	static inline Fimg* outlineSpriteSheet;
};