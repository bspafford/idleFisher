#include "animation.h"
#include "main.h"
#include "timer.h"

#include <functional>
#include <iostream>

animation::animation(std::string spriteSheetPath, int cellWidth, int cellHeight, std::unordered_map<std::string, animDataStruct> animData, bool useCharLoc, vector loc) {
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->animData = animData;
	this->loc = loc;
	this->useCharLoc = useCharLoc;

	spriteSheet = new Fimg("./images/" + spriteSheetPath, loc);
	cellWidthNum = spriteSheet->w / cellWidth;
	cellHeightNum = spriteSheet->h / cellHeight;
	spriteSheet->w = cellWidth;
	spriteSheet->h = cellHeight;

	animTimer = new timer();
	animTimer->addCallback(this, &animation::animCallBack);
}

animation::animation(Fimg* spriteSheetImg, int cellWidth, int cellHeight, std::unordered_map<std::string, animDataStruct> animData, bool useCharLoc, vector loc) {
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->animData = animData;
	this->loc = loc;
	this->useCharLoc = useCharLoc;

	SDL_Rect source = { 0, 0, spriteSheetImg->w, spriteSheetImg->h };
	spriteSheet = new Fimg(spriteSheetImg->texture, &source, loc); // create new own instance of image
	spriteSheet->surface = spriteSheetImg->surface;
	spriteSheet->loc = loc;
	cellWidthNum = spriteSheet->w / cellWidth;
	cellHeightNum = spriteSheet->h / cellHeight;
	spriteSheet->w = cellWidth;
	spriteSheet->h = cellHeight;

	animTimer = new timer();
	animTimer->addCallback(this, &animation::animCallBack);
}

animation::~animation() {
	stop();
	delete animTimer;
	animTimer = nullptr;
	eventCallback_ = nullptr;
	finishedCallback_ = nullptr;
	frameCallback_ = nullptr;
}

void animation::draw(SDL_Renderer* renderer) {
	if (!spriteSheet)
		return;

	spriteSheet->draw(renderer, useCharLoc);
	//if (currAnim == "normal" || currAnim == "hover")
	//	std::cout << currFrameLoc.x << ", " << cellWidthNum - 1 << std::endl;

	int frameNum = calcFrameDistance(true);
}

void animation::start() {
	// reset frame back to beginning
	currFrameLoc = animData[currAnim].start;
	bStopped = false;
	bFinished = false;

	if (animTimer)
		animTimer->start(animData[currAnim].duration);

	// set source
	source = { int(currFrameLoc.x * cellWidth), int(currFrameLoc.y * cellHeight), cellWidth, cellHeight };
	if (spriteSheet)
		spriteSheet->setSourceRect(&source);

	if (frameCallback_)
		frameCallback_(calcFrameDistance(true));
}

void animation::stop() {
	currFrameLoc = animData[currAnim].start;
	bStopped = true;
	bFinished = false;
	animTimer->stop();
}

void animation::setAnimation(std::string name, bool instantUpdate) {
	int frameNum = calcFrameDistance(true);

	currAnim = name;

	// if prev animation was on a greater frame than current anim then reset frameNum
	if (frameNum > calcFrameDistance(false))
		frameNum = 0;

	currFrameLoc = animData[currAnim].start + vector{ float(frameNum), 0 };

	if (currFrameLoc.x > cellWidthNum - 1)
		currFrameLoc = animData[currAnim].start;

	source = { int(currFrameLoc.x * cellWidth), int(currFrameLoc.y * cellHeight), cellWidth, cellHeight };

	if (spriteSheet)
		spriteSheet->setSourceRect(&source);
}

void animation::animCallBack() {
	if (bStopped)
		return;

	currFrameLoc.x++;
	
	// if out side of img rect || if past the end loc
	if (currFrameLoc.x > animData[currAnim].end.x && currFrameLoc.y == animData[currAnim].end.y) {
		currFrameLoc = animData[currAnim].start;
		if (!animData[currAnim].loop) {
			if (animTimer)
				animTimer->stop();
			bFinished = true;
		}
		if (finishedCallback_) {
			finishedCallback_();
			if (bStopped)
				return;
		}
	}
	
	if (frameCallback_) {
		//std::cout << "start: " << animData[currAnim].start << ", end: " << animData[currAnim].end << ", frame: " << currFrameLoc << std::endl;
		frameCallback_(calcFrameDistance(true));
		//frameCallback_(frameNum % animList.size());
	}

	if (eventCallback_ && calcFrameDistance(true) == eventFrameNum - 1)
		eventCallback_();

	source = { int(currFrameLoc.x * cellWidth), int(currFrameLoc.y * cellHeight), cellWidth, cellHeight };
	spriteSheet->setSourceRect(&source);

	if (!bFinished)
		animTimer->start(animData[currAnim].duration);
}

bool animation::finished() {
	return bFinished;
}

int animation::calcFrameDistance(bool getFrameNum) {
	vector start = animData[currAnim].start;
	vector end;
	if (getFrameNum)
		end = currFrameLoc;
	else
		end = animData[currAnim].end;

	//int spriteSheetWidth = spriteSheet->w / cellWidth;

	//int startIndex = start.y * spriteSheetWidth + start.x;
	//int endIndex = end.y * spriteSheetWidth + end.x;
	//return endIndex - startIndex;

	return end.x - start.x;
}

void animation::setLoc(vector loc) {
	this->loc = loc;
	if (spriteSheet)
		spriteSheet->loc = loc;
}

vector animation::getLoc() {
	return loc;
}