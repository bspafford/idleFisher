#include "equippedWidget.h"
#include "main.h"
#include "saveData.h"
#include "hoverBox.h"
#include "button.h"
#include "text.h"
#include "achievementWidget.h"

UequippedWidget::UequippedWidget() {
	charImg = std::make_unique<Image>("./images/character/idleSE1.png", vector{ 0, 0 }, false);

	fishingRodEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);
	baitEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);

	headEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);
	shirtEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);
	legsEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);
	shoesEquipSlot = std::make_unique<Image>("./images/widget/thumbnailBackground.png", vector{ 0, 0 }, false);

	update();
}

UequippedWidget::~UequippedWidget() {

}

void UequippedWidget::draw(Shader* shaderProgram) {
	charImg->draw(shaderProgram);
	fishingRodEquipSlot->draw(shaderProgram);
	baitEquipSlot->draw(shaderProgram);

	headEquipSlot->draw(shaderProgram);
	shirtEquipSlot->draw(shaderProgram);
	legsEquipSlot->draw(shaderProgram);
	shoesEquipSlot->draw(shaderProgram);

	if (fishingRodEquip)
		fishingRodEquip->draw(shaderProgram);
	if (baitEquip)
		baitEquip->draw(shaderProgram);
}

void UequippedWidget::update() {
	fishingRodEquip = std::make_unique<Image>("./images/widget/thumbnails/fishingRodThumbnail1.png", vector{0, 0}, false);
	fishingRodEquip->setLoc(fishingRodEquipSlot->getLoc());

	baitEquip = std::make_unique<Image>(SaveData::saveData.equippedBait.thumbnail, vector{ 0, 0 }, false);
	baitEquip->setLoc(baitEquipSlot->getLoc());
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

	charImg->setLoc(offset + getSize() / 2 - charImg->getSize() / 2);

	fishingRodEquipSlot->setLoc(offset);
	baitEquipSlot->setLoc(offset + vector{ size.x * 3, 0 });

	headEquipSlot->setLoc(offset + vector{ 0, size.y });
	shirtEquipSlot->setLoc(offset + vector{ 0, size.y * 2 });
	legsEquipSlot->setLoc(offset + vector{ size.x * 3, size.y });
	shoesEquipSlot->setLoc(offset + vector{ size.x * 3, size.y * 2 });

	if (fishingRodEquip)
		fishingRodEquip->setLoc(fishingRodEquipSlot->getLoc());
	if (baitEquip)
		baitEquip->setLoc(baitEquipSlot->getLoc());
	if (headEquip)
		headEquip->setLoc(headEquipSlot->getLoc());
	if (shirtEquip)
		shirtEquip->setLoc(shirtEquipSlot->getLoc());
	if (legsEquip)
		legsEquip->setLoc(legsEquipSlot->getLoc());
	if (shoesEquip)
		shoesEquip->setLoc(shoesEquipSlot->getLoc());
}

bool UequippedWidget::mouseOverSlot(std::string &name, std::string &description) {
	bool hovering = false;
	if (fishingRodEquip && mouseOver(fishingRodEquip.get())) {
		name = "Fishing Rod"; // SaveData::saveData.equippedFishingRod.name;
		description = "Fishing Rod"; // SaveData::saveData.equippedFishingRod.description;
		hovering = true;
	} else if (baitEquip && mouseOver(baitEquip.get())) {
		name = SaveData::saveData.equippedBait.name;
		description = SaveData::saveData.equippedBait.description;
		hovering = true;
	} else if (headEquip && mouseOver(headEquip.get())) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (shirtEquip && mouseOver(shirtEquip.get())) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (legsEquip && mouseOver(legsEquip.get())) {
		//hoverBox->setInfo();
		hovering = true;
	} else if (shoesEquip && mouseOver(shoesEquip.get())) {
		//hoverBox->setInfo();
		hovering = true;
	}

	if (hovering)
		return true;
	return false;
}

bool UequippedWidget::mouseOver(Image* img) {
	vector mousePos = Main::mousePos;
	vector newLoc = img->getLoc() + parent->ogLoc;
	vector size = img->getSize();
	if (mousePos.x >= newLoc.x && mousePos.x <= newLoc.x + size.x && mousePos.y >= newLoc.y && mousePos.y <= newLoc.y + size.y)
		return true;
	return false;
}