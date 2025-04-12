#include "achievementBox.h"

#include "main.h"
#include "saveData.h"
#include "timer.h"

UachievementBox::UachievementBox(int id) {
	this->id = id;

	if (!rotateTimer) {
		rotateTimer = new timer();
		rotateTimerSpeed = 4;
		rotateTimer->addUpdateCallback(UachievementBox::rotateUpdate);
		rotateTimer->addCallback(UachievementBox::finishRotateTimer);
		rotateTimer->start(rotateTimerSpeed);
	}

	achievementStruct = &SaveData::data.achievementData[id];
	saveAchievementStruct = &SaveData::saveData.achievementList[id];

	if (saveAchievementStruct->unlocked) {
		icon = new Fimg("./images/widget/achievementIcons/achievementIcon" + std::to_string(id + 1) + ".png");
	} else
		icon = new Fimg("./images/widget/achievementIcons/locked.png");

	startSize = { float(icon->w), float(icon->h) };
}

UachievementBox::~UachievementBox() {
	delete icon;
	icon = nullptr;
}

void UachievementBox::rotateUpdate() {
	float percent = rotateTimer->getTime() / rotateTimer->getMaxTime();

	rot = sin(percent * 2 * M_PI) * 5;
}

void UachievementBox::finishRotateTimer() {
	rotateTimer->start(rotateTimerSpeed);
}

void UachievementBox::updateAchievementImage() {
	delete icon;
	if (saveAchievementStruct->unlocked) {
		icon = new Fimg("./images/widget/achievementIcons/achievementIcon" + std::to_string(id + 1) + ".png");
	} else
		icon = new Fimg("./images/widget/achievementIcons/locked.png");
}

void UachievementBox::draw(SDL_Renderer* renderer) {
	if (icon) {
		if (mouseOver()) {
			isMouseOver = true;
			// icon->loc = loc - vector{ 0, stuff::pixelSize };

			float scale = 1.3;
			icon->w = startSize.x * scale;
			icon->h = startSize.y * scale;

			if (saveAchievementStruct->unlocked)
				icon->setRotation(rot);

			vector test = (icon->getSize() / stuff::pixelSize);
			icon->loc = loc - (icon->getSize() / stuff::pixelSize - startSize) / 2 * stuff::pixelSize;
		} else {
			icon->loc = loc;
			icon->w = startSize.x;
			icon->h = startSize.y;
		}
		icon->draw(renderer, false);
	}
}

vector UachievementBox::getSize() {
	if (icon)
		//return { float(icon->w * stuff::pixelSize), float(icon->h * stuff::pixelSize) };
		return startSize * stuff::pixelSize;
	return { 0, 0 };
}

bool UachievementBox::mouseOver() {
	vector mousePos = Main::mousePos;
	vector ogLoc = parent->ogLoc;
	vector parentSize = parent->parent->getSize();
	vector newLoc = ogLoc + icon->loc;

	// if mouse in parent
	if (mousePos.x >= ogLoc.x && mousePos.x <= ogLoc.x + parentSize.x && mousePos.y >= ogLoc.y && mousePos.y <= ogLoc.y + parentSize.y)
		if (mousePos.x >= newLoc.x && mousePos.x <= newLoc.x + icon->w * stuff::pixelSize && mousePos.y >= newLoc.y && mousePos.y <= newLoc.y + icon->h * stuff::pixelSize)
			return true;
	return false;
}