#include "fishSchool.h"
#include "animation.h"
#include "timer.h"
#include "worlds.h"

AfishSchool::AfishSchool(vector loc) {
	this->loc = loc;

	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "fishSchool", {{0, 0}, {6, 0}, .1, true} });
	anim = new animation("fishSchool/fishSchool.png", 48, 26, animData, true, loc);
	anim->setAnimation("fishSchool");
	anim->start();

	fishNum = math::randRange(1, 5);
	lifeTimer = new timer();
	lifeTimer->start(math::randRange(30, 60));
	lifeTimer->addCallback(this, &AfishSchool::remove);
}

AfishSchool::~AfishSchool() {
	delete anim;
	delete lifeTimer;
	anim = nullptr;
	lifeTimer = nullptr;
}

void AfishSchool::draw(SDL_Renderer* renderer) {
	anim->draw(renderer);
}

AfishSchool* AfishSchool::pointInSchool(vector worldPoint) {
	// i guess calc if in ellipse
	float h = loc.x + anim->cellWidth / 2;
	float k = loc.y + anim->cellHeight / 2;
	float a = anim->cellWidth / 2;
	float b = anim->cellHeight / 2;
	float y = (((worldPoint.x - h) * (worldPoint.x - h)) / (a * a)) + (((worldPoint.y - k) * (worldPoint.y - k)) / (b * b));
	if (y <= 1)
		return this;
	return nullptr;
}

void AfishSchool::removeFishNum() {
	fishNum--;
	if (fishNum <= 0)
		remove();
}

void AfishSchool::setLoc(vector loc) {
	this->loc = loc;
	if (anim)
		anim->setLoc(loc);
}

void AfishSchool::remove() {
	world::removeFishSchool(this);
}

