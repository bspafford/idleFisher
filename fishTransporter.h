#pragma once

#include "npc.h"
#include "math.h"

class AautoFisher;
class timer;
class UprogressBar;
class animation;
class AStar;
class text;

class AfishTransporter : public npc {
public:
	AfishTransporter(vector loc);
	~AfishTransporter();

	void draw(SDL_Renderer* renderer) override;
	void update(float deltaTime);
	void startPathFinding();
	void calcIdleProfits(float timeDiff);
	double getMaxHoldNum();
	float getSpeed();
	float getCollectionSpeed();
	void upgrade(FsaveMechanicStruct* saveMechanicStruct);
private:
	void setupCollision() override;
	void setAnimation();
	// if getMaxTime is true it will return the maximum amount of time it will take for the fish transporter to collect its max fish
	float calcCollectTimer(AautoFisher* autoFisher, bool getMaxTime = false);
	void finishCollectTimer();
	void collectTimerUpdate();
	void collectFish(AautoFisher* autoFisher);
	double calcCurrencyHeld();
	// sorts list from biggest index to smallest
	void sortFishList(std::vector<FsaveFishData>& list);
	void addFishtoHeld(FsaveFishData* fish, double addNum);
	vector calcGoTo(int autoFisherIndex);
	void setLoc(vector loc) override;
	void click() override;

	AStar* Astar;

	UprogressBar* progressBar;

	float speed = 50;
	float collectionSpeed = 5;

	int autoFisherIndex;

	animation* anim;
	animation* fishPileAnim;
	bool canMove = true;
	vector moveDir;
	vector prevMove;
	timer* collectTimer;
	text* fullnessText;

	std::vector<FsaveFishData> holding;
	double maxHoldNum = 100;

	// animations
	std::vector<std::vector<std::string>> walkAnimWheel;
	std::vector<std::string> currAnim;
	std::vector<std::string> walkE, walkNE, walkN, walkNW, walkW, walkSW, walkS, walkSE;

	// idle
	std::vector<std::string> idleE, idleNE, idleN, idleNW, idleW, idleSW, idleS, idleSE;
	std::vector<std::vector<std::string>> idleAnimWheel;

	std::string fullnessString = "empty";

};