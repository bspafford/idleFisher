#include "fishBox.h"
#include "stuff.h"
#include "shortNumbers.h"
#include "saveData.h"

#include "journal.h"
#include "text.h"
#include "button.h"

UfishBox::UfishBox(Ujournal* ref, FfishData* fishData, FsaveFishData* saveFishData) {
	this->ref = ref;

	this->fishData = fishData;
	this->saveFishData = saveFishData;
	
	std::string fishThumbnail = fishData->thumbnail;
	fishThumbnail.erase(0, 9);
	fishButton = new Ubutton(ref, fishThumbnail, 16, 16, 1, { 0, 0 }, false, false);
	fishButton->addCallback(this, &UfishBox::openFishPage);
	name = new text(fishData->name, "straightDark", { 0, 0 }, false, false, textAlign::center);
	checkMark = new Fimg("./images/widget/check.png");

	if (name && fishButton && !saveFishData->unlocked) {
		name->setText("???");
		Fimg* img = fishButton->getButtonImg();
		if (img)
			img->setImgColorMod(0, 0, 0);
		fishButton->enable(false);
	}
}

UfishBox::~UfishBox() {
	delete fishButton;
	delete name;
	fishButton = nullptr;
	name = nullptr;
}

void UfishBox::draw(SDL_Renderer* renderer) {
	fishButton->draw(renderer);
	name->draw(renderer);
	checkMark->draw(renderer, false);
}

void UfishBox::openFishPage() {
	ref->openFishPage(fishData, saveFishData);
}

void UfishBox::setLoc(vector loc) {
	__super::setLoc(loc);

	//img->loc = loc;
	if (fishButton) {
		vector buttonSize = fishButton->getSize();
		fishButton->setLoc(loc + vector{ 22.f * stuff::pixelSize - buttonSize.x / 2, 17.f * stuff::pixelSize - buttonSize.y / 2 });
	}
	if (name)
		name->setLoc(loc + vector{ 23, 26 } * stuff::pixelSize);

	bool hasAllStars = saveFishData->totalNumOwned[1] != 0 && saveFishData->totalNumOwned[2] != 0 && saveFishData->totalNumOwned[3] != 0;
	bool hasBiggestSize = saveFishData->biggestSizeCaught == fishData->maxSize;
	if (checkMark && saveFishData->unlocked && hasAllStars && hasBiggestSize)
		checkMark->loc = loc + vector{ 25, 20 } * stuff::pixelSize;
}

void UfishBox::updateUI() {
	if (!saveFishData->unlocked)
		return;

	//img->setImgColorMod(255, 255, 255);
	fishButton->getButtonImg()->setImgColorMod(255, 255, 255);
	fishButton->enable(true);
	name->setText(fishData->name);
}