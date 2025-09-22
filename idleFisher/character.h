#pragma once

#include <string>

#include "saveData.h"

class saveData;
class animation;
class collision;
class timer;
class AfishSchool;
struct Fcollision;
class Shader;
class Image;


class Acharacter {
public:
	Acharacter();

	void draw(Shader* shaderProgram);
	void drawFishingLine(Shader* shaderProgram);
	void Update(float deltaTime);

	// get the world location at the players feet
	static vector getCharLoc();

	void leftClick();
	void fishing();
	void stopFishing();

	void setCanMove(bool move);
	bool getCanMove();

	void bobberCatchAnim();
	void bobberBobAnim();

	void calcFishSchoolUpgrades();
	AfishSchool* bobberInFishSchool();
	static inline float fishSchoolMultiplier = 1.f; // increases the fish caught
	static inline float fishTimeMultiplier = 1.f; // reduces the catch time
	AfishSchool* currFishSchool; // if cast in a fishSchool

	vector getCharScreenLoc();
	/*
	float bobberTime = 0;
	float bobberTimer = .25;
	bool doingBobberAnim = false;
	*/

	// fish stuff
	FfishData calcFish(int& quality, float& fishSize);
	std::vector<std::vector<float>> calcFishProbability(std::vector<FfishData> fishData);
	void comboExceeded();

	static inline animation* anim;
	animation* fishingRod;


	// setup fish image
	Image* fishImg;
	bool showFish = false;

	FfishData currFish;
	int currFishQuality;
	float currFishSize;

private:
	bool canMove = true;
public:
	vector prevMove = { 1, -1 };

	static inline Image* bobberImg;
	static inline Image* bobberWaterOverlay;
	static inline animation* bobberWaterAnimFront;
	static inline animation* bobberWaterAnimBack;

	float speed = 425.f / 5.f;

	void move(vector dir, float deltaTime);

	vector moveDir;

	inline static bool isFishing = false;
	inline static vector bobberLoc;
	vector tempBobberLoc;

	// temp
	float minFishTime = 5;
	float maxFishTime = 10;

	float catchTimer = 0;

	timer* fishingTimer;
	timer* bobberCatchTimer;
	timer* bobberBobTimer;
	float bobTime = 1.5;

	void animFinished();
	// temp
	std::vector<vector> fishLineTipLocs;
	void setFishingTipLoc(int frame);

	// stops player from moving if they start fishing
	bool fishingStop = false;

	int comboNum = 1;
	// temp
	bool comboUnlocked = true;
	timer* comboOvertimer;
	void comboOvertimeFinished();

	static inline Fcollision* col;
	static void setPlayerColPoints();

	// fishing line stuff
	static inline vector fishingTipLoc = { 69, 60 };
	static inline bool showFishingLine = false;

	// returns false if the player doesn't have a strong enough fishing rod for the world
	// so if world 2 requires 60 power for first fish but you only have a 50 power fishing rod, then it will return false
	bool canCatchWorldFish();

	void premiumFishBuff();

	// if going can't catch golden fihs
	timer* premiumCatchTimer;
	bool canCatchPremium = true;
	void setCatchPremium();

	void equipFishingRod(FfishingRodStruct* fishingRod);
	void equipBait(FbaitStruct* bait);
};