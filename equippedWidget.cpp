#include "equippedWidget.h"
#include "main.h"
#include "saveData.h"
#include "hoverBox.h"
#include "button.h"
#include "text.h"
#include "achievementWidget.h"

UequippedWidget::UequippedWidget() {
	charImg = new Fimg("./images/character/idleSE1.png");

	fishingRodEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");
	baitEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");

	headEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");
	shirtEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");
	legsEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");
	shoesEquipSlot = new Fimg("./images/widget/thumbnailBackground.png");

	update();
}

UequippedWidget::~UequippedWidget() {
	delete charImg;
	delete fishingRodEquipSlot;
	delete baitEquipSlot;
	delete headEquipSlot;
	delete shirtEquipSlot;
	delete legsEquipSlot;
	delete shoesEquipSlot;
	delete fishingRodEquip;
	delete baitEquip;
	delete headEquip;
	delete shirtEquip;
	delete legsEquip;
	delete shoesEquip;
	charImg = nullptr;
	fishingRodEquipSlot = nullptr;
	baitEquipSlot = nullptr;
	headEquipSlot = nullptr;
	shirtEquipSlot = nullptr;
	legsEquipSlot = nullptr;
	shoesEquipSlot = nullptr;
	fishingRodEquip = nullptr;
	baitEquip = nullptr;
	headEquip = nullptr;
	shirtEquip = nullptr;
	legsEquip = nullptr;
	shoesEquip = nullptr;
}

void UequippedWidget::draw(SDL_Renderer* renderer) {
	charImg->draw(renderer, false);
	fishingRodEquipSlot->draw(renderer, false);
	baitEquipSlot->draw(renderer, false);

	headEquipSlot->draw(renderer, false);
	shirtEquipSlot->draw(renderer, false);
	legsEquipSlot->draw(renderer, false);
	shoesEquipSlot->draw(renderer, false);

	if (fishingRodEquip)
		fishingRodEquip->draw(renderer, false);
	if (baitEquip)
		baitEquip->draw(renderer, false);
}

void UequippedWidget::update() {
	delete fishingRodEquip;
	fishingRodEquip = new Fimg(SaveData::saveData.equippedFishingRod.imgPath);
	fishingRodEquip->loc = fishingRodEquipSlot->loc;

	delete baitEquip;
	baitEquip = new Fimg(SaveData::saveData.equippedBait.thumbnail);
	baitEquip->loc = baitEquipSlot->loc;
}

vector UequippedWidget::getSize() {
	vector size = fishingRodEquipSlot->getSize() + stuff::pixelSize;
	return vector{ size.x * 4, size.y * 3 };
	// return vector{ 16 * 3, 16 * 4 + 4 } * stuff::pixelSize;
}

void UequippedWidget::setLoc(vector loc) {
	__super::setLoc(loc);

	vector parentSize = parent->getSize();
	vector offset = loc + vector{ (parentSize / 2 - getSize() / 2).x, 0 };

	vector size = fishingRodEquipSlot->getSize() + stuff::pixelSize;

	charImg->loc = offset + getSize() / 2 - charImg->getSize() / 2;

	fishingRodEquipSlot->loc = offset;
	baitEquipSlot->loc = offset + vector{ size.x * 3, 0 };

	headEquipSlot->loc = offset + vector{ 0, size.y };
	shirtEquipSlot->loc = offset + vector{ 0, size.y * 2 };
	legsEquipSlot->loc = offset + vector{ size.x * 3, size.y };
	shoesEquipSlot->loc = offset + vector{ size.x * 3, size.y * 2 };

	if (fishingRodEquip)
		fishingRodEquip->loc = fishingRodEquipSlot->loc;
	if (baitEquip)
		baitEquip->loc = baitEquipSlot->loc;
	if (headEquip)
		headEquip->loc = headEquipSlot->loc;
	if (shirtEquip)
		shirtEquip->loc = shirtEquipSlot->loc;
	if (legsEquip)
		legsEquip->loc = legsEquipSlot->loc;
	if (shoesEquip)
		shoesEquip->loc = shoesEquipSlot->loc;
}

bool UequippedWidget::mouseOverSlot(std::string &name, std::string &description) {
	bool hovering = false;
	if (fishingRodEquip && mouseOver(fishingRodEquip)) {
		name = SaveData::saveData.equippedFishingRod.name;
		description = SaveData::saveData.equippedFishingRod.description;
		hovering = true;
	} else if (baitEquip && mouseOver(baitEquip)) {
		name = SaveData::saveData.equippedBait.name;
		description = SaveData::saveData.equippedBait.description;
		hovering = true;
	} else if (headEquip && mouseOver(headEquip)) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (shirtEquip && mouseOver(shirtEquip)) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (legsEquip && mouseOver(legsEquip)) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (shoesEquip && mouseOver(shoesEquip)) {
		//hoverBox->setInfo();
		hovering = true;
	}

	if (hovering)
		return true;
	return false;
}

bool UequippedWidget::mouseOver(Fimg* img) {
	vector mousePos = Main::mousePos;
	vector newLoc = img->loc + parent->ogLoc;
	vector size = img->getSize();
	if (mousePos.x >= newLoc.x && mousePos.x <= newLoc.x + size.x && mousePos.y >= newLoc.y && mousePos.y <= newLoc.y + size.y)
		return true;
	return false;
}