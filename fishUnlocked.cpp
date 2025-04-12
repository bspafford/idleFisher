#include "fishUnlocked.h"

#include "main.h"
#include "saveData.h"
#include "timer.h"
#include "widget.h"
#include "animation.h"
#include "text.h"

UfishUnlocked::UfishUnlocked() {
	bannerImg = new Fimg("./images/widget/fishUnlocked.png");

	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "normal", {{0, 0}, {4, 0}, .1, false} });
	animData.insert({ "reverse", {{0, 1}, {4, 1}, .1, false} });
	anim = new animation("widget/fishUnlockedSpritesheet.png", 135, 63, animData, false, { 0, 0 }); // 116*1.5, 67*1.5
	anim->setAnimation("normal");
	anim->addFinishedCallback(this, &UfishUnlocked::finished);

	finishedTimer = new timer();
	finishedTimer->addCallback(this, &UfishUnlocked::reverse);

	visible = false;
}

UfishUnlocked::~UfishUnlocked() {
	delete bannerImg;
	delete finishedTimer;
	delete thumbnail;
	delete name;
	bannerImg = nullptr;
	finishedTimer = nullptr;
	thumbnail = nullptr;
	name = nullptr;
}

void UfishUnlocked::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	if (anim->finished()) {
		bannerImg->draw(renderer, false);
		thumbnail->draw(renderer, false);
		if (name)
			name->draw(renderer);
	} else
		anim->draw(renderer);
}

void UfishUnlocked::start(FfishData fish) {
	visible = true;
	if (thumbnail)
		delete thumbnail;
	thumbnail = new Fimg(fish.thumbnail);
	finishedTimer->start(4);
	setupLocs();

	anim->setAnimation("normal");
	anim->start();
}

void UfishUnlocked::start(FachievementStruct* achievementData) {
	visible = true;
	if (thumbnail)
		delete thumbnail;
	thumbnail = new Fimg("./images/widget/achievementIcons/achievementIcon" + std::to_string(achievementData->id + 1) + ".png");;
	if (name)
		delete name;
	name = new text(achievementData->name, "straight", { 0, 0 });
	name->setLineLength(50 * stuff::pixelSize);

	finishedTimer->start(4);
	setupLocs();

	anim->setAnimation("normal");
	anim->start();
}

void UfishUnlocked::setupLocs() {
	__super::setupLocs();

	vector center = { Main::screenWidth / 2.f - bannerImg->w / 2 * stuff::pixelSize, 17 * stuff::pixelSize};
	bannerImg->loc = center;
	//anim->setLoc(center - vector{ 20, 26 } * stuff::pixelSize);
	anim->setLoc(bannerImg->loc);

	if (thumbnail)
		thumbnail->loc = bannerImg->loc + vector{ 30, 10 } * stuff::pixelSize;// center + vector{ 17 + 20, 3 + 26 } *stuff::pixelSize;
	if (name)
		name->setLoc(bannerImg->loc + vector{ 58, 14 } * stuff::pixelSize);//center + vector{ 42, 5 } * stuff::pixelSize);
}

void UfishUnlocked::reverse() {
	anim->setAnimation("reverse");
	anim->start();
}

void UfishUnlocked::finished() {
	if (anim->currAnim == "reverse")
		visible = false;
}