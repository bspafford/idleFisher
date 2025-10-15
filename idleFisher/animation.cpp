#include "animation.h"
#include "main.h"
#include "timer.h"
#include "Image.h"
#include "shaderClass.h"

#include <functional>
#include <iostream>

animation::animation(std::string spriteSheetPath, int cellWidth, int cellHeight, std::unordered_map<std::string, animDataStruct> animData, bool useWorldLoc, vector loc) {
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->animData = animData;
	this->loc = loc;
	this->useWorldLoc = useWorldLoc;

	spriteSheet = std::make_shared<Image>("./images/" + spriteSheetPath, loc, useWorldLoc);
	cellWidthNum = spriteSheet->w / cellWidth;
	cellHeightNum = spriteSheet->h / cellHeight;

	animTimer = std::make_unique<timer>();
	animTimer->addCallback(this, &animation::animCallBack);
}

animation::animation(std::shared_ptr<Image> spriteSheetImg, int cellWidth, int cellHeight, std::unordered_map<std::string, animDataStruct> animData, bool useWorldLoc, vector loc) {
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;
	this->animData = animData;
	this->loc = loc;
	this->useWorldLoc = useWorldLoc;

	std::shared_ptr<Rect> source = std::make_shared<Rect>(0.f, 0.f, spriteSheetImg->w, spriteSheetImg->h);
	spriteSheet = std::make_shared<Image>(spriteSheetImg, source, loc, useWorldLoc); // create own instance of image

	cellWidthNum = spriteSheet->w / cellWidth;
	cellHeightNum = spriteSheet->h / cellHeight;

	animTimer = std::make_unique<timer>();
	animTimer->addCallback(this, &animation::animCallBack);
}

animation::~animation() {
	stop();
	animTimer = nullptr;
	eventCallback_ = nullptr;
	finishedCallback_ = nullptr;
	frameCallback_ = nullptr;
}

void animation::draw(Shader* shaderProgram) {
	spriteSheet->draw(shaderProgram);
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
	source = std::make_shared<Rect>(currFrameLoc.x * cellWidth, currFrameLoc.y * cellHeight, float(cellWidth), float(cellHeight));
	spriteSheet->setSourceRect(source);

	if (frameCallback_)
		frameCallback_(calcFrameDistance(true));
}

void animation::stop() {
	currFrameLoc = animData[currAnim].start;
	bStopped = true;
	bFinished = false;
	if (animTimer)
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

	source = std::make_shared<Rect>(currFrameLoc.x * cellWidth, currFrameLoc.y * cellHeight, float(cellWidth), float(cellHeight));
	spriteSheet->setSourceRect(source);

	// updates loc, so it isn't offset by the change in source
	setLoc(loc);
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

	source = std::make_shared<Rect>(currFrameLoc.x * cellWidth, currFrameLoc.y * cellHeight, float(cellWidth), float(cellHeight));
	spriteSheet->setSourceRect(source);

	if (!bFinished && animTimer)
		animTimer->start(animData[currAnim].duration);
}

void animation::addFinishedCallback(void (*callback) ()) {
	finishedCallback_ = callback;
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
	spriteSheet->setLoc(loc);

}

vector animation::getLoc() {
	return loc;
}