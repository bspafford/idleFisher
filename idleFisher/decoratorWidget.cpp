#include "decoratorWidget.h"
#include "decorator.h"
#include "decorationBox.h"
#include "vaultPlacedItems.h"
#include "main.h"
#include "saveData.h"
#include "scrollBox.h"
#include "wrapBox.h"
#include "button.h"
#include "text.h"
#include "Rectangle.h"

UdecoratorWidget::UdecoratorWidget(Adecorator* parent) {
	/*
	this->parent = parent;

	vector loc = { .4 * stuff::screenSize.x, .2 * stuff::screenSize.y };
	vector size = { .45 * stuff::screenSize.x, .6 * stuff::screenSize.y };

	scrollBox = new UscrollBox();
	wrapBox = new UwrapBox(loc, size);
	for (int i = 0; i < SaveData::data.vaultUnlockData.size(); i++) {
		FvaultUnlocksStruct* curr = &SaveData::data.vaultUnlockData[i];
		FsaveVaultUnlocksStruct* saveCurr = &SaveData::saveData.vaultUnlockList[i];
		UdecorationBox* decorationBox = new UdecorationBox(this, wrapBox, curr, saveCurr);
		wrapBox->addChild(decorationBox);
	}
	scrollBox->addChild(wrapBox, wrapBox->getOverflowSize().y);

	closeButton = new Ubutton(this, "autoFisher/moreUI/xButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &UdecoratorWidget::removeFromViewport);
	npcImg = new Image(parent->getCharImg()->path, { 100, 100 }, false);
	npcImg->w *= 1;
	npcImg->h *= 1;

	name = new text(" ", "straight", { 0,0 });
	description = new text(" ", "straight", { 0,0 });
	nameHolder = new verticalBox();
	nameHolder->addChild(name, 6 * stuff::pixelSize);
	nameHolder->addChild(description, 4 * stuff::pixelSize);

	finishPlacingButton = new Ubutton(this, "widget/decorator/finishButton.png", 31, 13, 2, { 0, 0 }, false, false);
	finishPlacingButton->addCallback(this, &UdecoratorWidget::finishPlaceMode);
	moveButton = new Ubutton(this, "widget/decorator/moveButton.png", 27, 13, 2, { 0, 0 }, false, false);
	moveButton->addCallback(this, &UdecoratorWidget::moveMode);

	setupLocs();
	*/
}

UdecoratorWidget::~UdecoratorWidget() {

}

void UdecoratorWidget::draw(Shader* shaderProgram) {
	/*
	if (!visible)
		return;

	__super::draw(shaderProgram);

	if (AvaultPlacedItems::inPlacingMode) {
		finishPlacingButton->draw(shaderProgram);
	} else {
		SDL_SetRenderDrawColor(shaderProgram, 10, 10, 10, 255);
		SDL_RenderFillRect(shaderProgram, upgradeBackground);
		SDL_RenderFillRect(shaderProgram, npcBackground);
		SDL_RenderFillRect(shaderProgram, infoBackground);

		npcImg->draw(shaderProgram);

		nameHolder->draw(shaderProgram);
		scrollBox->draw(shaderProgram);

		closeButton->draw(shaderProgram);
		moveButton->draw(shaderProgram);
	}
	*/
}

void UdecoratorWidget::addedToViewport() {
	AvaultPlacedItems::inPlacingMode = false;

	for (widget* child : wrapBox->getAllChildren()) {
		static_cast<UdecorationBox*>(child)->updateWidget();
	}
}

void UdecoratorWidget::placeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock) {
	AvaultPlacedItems::inPlacingMode = true;

	AvaultPlacedItems::placeItem(vaultUnlock, saveVaultUnlock);
}

void UdecoratorWidget::removeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock) {
	AvaultPlacedItems::removeItem(vaultUnlock, saveVaultUnlock);
}

void UdecoratorWidget::moveMode() {
	AvaultPlacedItems::inPlacingMode = true;
}

void UdecoratorWidget::finishPlaceMode() {
	AvaultPlacedItems::inPlacingMode = false;

	for (widget* child : wrapBox->getAllChildren()) {
		static_cast<UdecorationBox*>(child)->updateWidget();
	}
}

void UdecoratorWidget::setupLocs() {
	/*
	__super::setupLocs();

	vector topLeft = { .4 * stuff::screenSize.x, .2 * stuff::screenSize.y };
	vector bottomRight = { .85 * stuff::screenSize.x, .8 * stuff::screenSize.y };
	upgradeBackground = new SDL_Rect{ int(topLeft.x), int(topLeft.y), int(bottomRight.x - topLeft.x), int(bottomRight.y - topLeft.y) };

	topLeft = { .15f * stuff::screenSize.x, .2f * stuff::screenSize.y };
	bottomRight = { .39f * stuff::screenSize.x, .5f * stuff::screenSize.y };
	npcBackground = new SDL_Rect{ int(topLeft.x), int(topLeft.y), int(bottomRight.x - topLeft.x), int(bottomRight.y - topLeft.y) };

	topLeft = { .15f * stuff::screenSize.x, .515f * stuff::screenSize.y };
	bottomRight = { .39f * stuff::screenSize.x, .8f * stuff::screenSize.y };
	infoBackground = new SDL_Rect{ int(topLeft.x), int(topLeft.y), int(bottomRight.x - topLeft.x), int(bottomRight.y - topLeft.y) };

	// centers the npc inside the npcImg thing
	topLeft = { .15f * stuff::screenSize.x, .2f * stuff::screenSize.y };
	vector centerX = { topLeft.x + (bottomRight.x - topLeft.x) / 2 - npcImg->w * 2, topLeft.y - 75 };
	npcImg->loc = centerX;

	// crops the bottom part of the npc img
	SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	npcImg->h = 25;
	npcImg->setSourceRect(source);

	// anchors the npc img to the bottom of the npc background
	int bottomY = npcBackground->y + npcBackground->h;
	float y = bottomY - npcImg->h * stuff::pixelSize;
	npcImg->loc = { npcImg->loc.x, y };

	//upgradeHolder->setLocAndSize({ float(upgradeBackground->x), float(upgradeBackground->y) }, { float(upgradeBackground->w), float(upgradeBackground->h) });
	//wrapBox->setLocAndSize({ float(upgradeBackground->x), float(upgradeBackground->y) }, { float(upgradeBackground->w), float(upgradeBackground->h) });

	wrapBox->setLocAndSize({ float(upgradeBackground->x), float(upgradeBackground->y) }, { float(upgradeBackground->w), float(upgradeBackground->h) });
	scrollBox->setLocAndSize({ float(upgradeBackground->x), float(upgradeBackground->y) }, { float(upgradeBackground->w), float(upgradeBackground->h) });
	scrollBox->changeChildHeight(wrapBox, wrapBox->getOverflowSize().y);

	vector closeButtonSize = closeButton->getSize();
	closeButton->setLoc({ float(upgradeBackground->x + upgradeBackground->w - closeButtonSize.x / 2), float(upgradeBackground->y - closeButtonSize.y / 2) });

	vector moveButtonSize = moveButton->getSize();
	moveButton->setLoc({ float(upgradeBackground->x + upgradeBackground->w - moveButtonSize.x - 2 * stuff::pixelSize), float(upgradeBackground->y + upgradeBackground->h - moveButtonSize.y - 2 * stuff::pixelSize) });

	nameHolder->setLocAndSize({ float(infoBackground->x) + 2 * stuff::pixelSize, float(infoBackground->y) + 2 * stuff::pixelSize }, { float(infoBackground->w), float(infoBackground->h) });
	description->setLineLength(infoBackground->w);

	vector buttonSize = finishPlacingButton->getSize();
	finishPlacingButton->setLoc(vector{ float(stuff::screenSize.x - 2 * stuff::pixelSize), float(stuff::screenSize.y) } - buttonSize - 2 * stuff::pixelSize);
	*/
}