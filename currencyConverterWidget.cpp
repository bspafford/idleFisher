#include "currencyConverterWidget.h"

#include "main.h"
#include "npc.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"

UcurrencyConverterWidget::UcurrencyConverterWidget(npc* parent) {
	this->parent = parent;

	upgradeHolder = new UscrollBox();
	closeButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &UcurrencyConverterWidget::removeFromViewport);
	npcImg = new Fimg("./images/widget/npcButtons/atm.png", {0, 0});

	name = new text(" ", "biggerStraight", { 0,0 });
	description = new text(" ", "straight", { 0,0 });
	nameHolder = new verticalBox();
	nameHolder->addChild(name, 8 * stuff::pixelSize);
	nameHolder->addChild(description, 4 * stuff::pixelSize);

	npcBackground = new Fimg("./images/widget/npcBackground.png");
	infoBackground = new Fimg("./images/widget/infoBackground.png");
	upgradeBackground = new Fimg("./images/widget/upgradeBackground.png");

	setup();
}

UcurrencyConverterWidget::~UcurrencyConverterWidget() {
	delete upgradeHolder;
	delete closeButton;
	delete npcImg;
	delete name;
	delete description;
	delete nameHolder;
	delete upgradeBackground;
	delete infoBackground;
	delete npcBackground;
	upgradeHolder = nullptr;
	closeButton = nullptr;
	npcImg = nullptr;
	name = nullptr;
	description = nullptr;
	nameHolder = nullptr;
	upgradeBackground = nullptr;
	infoBackground = nullptr;
	npcBackground = nullptr;
}

void UcurrencyConverterWidget::setup() {
	for (int i = 1; i < SaveData::data.currencyData.size() - 1; i++) {
		FcurrencyStruct* currData = &SaveData::data.currencyData[i];
		FsaveCurrencyStruct* currSaveData = &SaveData::saveData.currencyList[currData->id];
		if (currSaveData->unlocked && SaveData::saveData.currencyList[currData->id + 1].unlocked) { // if currency and next are unlocked
			UcurrencyConverterBox* upgradeBox = new UcurrencyConverterBox(this, currData, currSaveData);
			if (upgradeBox->buyButton)
				upgradeBox->buyButton->setParent(upgradeHolder);
			upgradeHolder->addChild(upgradeBox, upgradeBox->getSize().y);
		}
	}

	setupLocs();
}

void UcurrencyConverterWidget::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	if (!visible)
		return;

	upgradeBackground->draw(renderer, false);
	npcBackground->draw(renderer, false);
	if (infoBackground)
		infoBackground->draw(renderer, false);

	npcImg->draw(renderer, false);

	nameHolder->draw(renderer);
	upgradeHolder->draw(renderer);

	closeButton->draw(renderer);
}

void UcurrencyConverterWidget::setNameDescription(std::string nameString, std::string descriptionString) {
	name->setText(nameString);
	description->setText(descriptionString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name, name->getSize().y + stuff::pixelSize);
}

void UcurrencyConverterWidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { Main::screenWidth / 2, Main::screenHeight / 2 };
	vector topLeft = center - size / 2;
	npcBackground->loc = topLeft;
	if (infoBackground)
		infoBackground->loc = topLeft + vector{ 0, y };
	upgradeBackground->loc = topLeft + vector{ x, 0 };

	// crops the bottom part of the npc img
	//SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	//npcImg->h = 25;
	//npcImg->setSourceRect(source);

	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->loc = npcBackground->loc + vector{ npcBackgroundSize.x / 2, npcBackgroundSize.y } - vector{ npcSize.x / 2, npcSize.y } - vector{ 0, 3 * stuff::pixelSize };


	upgradeHolder->setLocAndSize(upgradeBackground->loc, vector{ float(upgradeBackground->w), float(upgradeBackground->h - 6) } *stuff::pixelSize);
	upgradeHolder->ogLoc = upgradeBackground->loc + vector{ 4, 3 } *stuff::pixelSize;

	vector closeButtonSize = closeButton->getSize();
	closeButton->setLoc({ float(upgradeBackground->loc.x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->loc.y - closeButtonSize.y / 2) });

	if (infoBackground) {
		nameHolder->setLocAndSize({ float(infoBackground->loc.x) + 6 * stuff::pixelSize, float(infoBackground->loc.y) + 6 * stuff::pixelSize }, vector{ float(infoBackground->w), float(infoBackground->h) } *stuff::pixelSize);
		name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
		description->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	}
}

void UcurrencyConverterWidget::addedToViewport() {
	std::cout << "added to viewport" << std::endl;

	// check to see if there are now more currencies unlocked
	// if so add the child
	// if not do nothing
	int unlockedNum = 0;
	//for (const FsaveCurrencyStruct currency : SaveData::saveData.currencyList) {
	for (int i = 1; i < SaveData::saveData.currencyList.size(); i++) {
		FsaveCurrencyStruct currency = SaveData::saveData.currencyList[i];
		std::cout << "currency: " << currency.id << ", " << currency.unlocked << std::endl;
		if (currency.unlocked)
			unlockedNum++;
	}

	int childListSize = upgradeHolder->childList.size();
	std::cout << "childListSize123: " << childListSize << ", " << unlockedNum << std::endl;
	if (childListSize != unlockedNum) {
		// add children needed
		// its possible for there to be multiple new currencies

		// loop through starting at i = childListSize - 1 to unlockedNum
		// then add child, child being currency[i]
		std::cout << "childListSize: " << childListSize << ", " << unlockedNum << std::endl;
		for (int i = childListSize; i < unlockedNum-1; i++) {
			FcurrencyStruct* currData = &SaveData::data.currencyData[i+1];
			FsaveCurrencyStruct* currSaveData = &SaveData::saveData.currencyList[currData->id];
			if (currSaveData->unlocked && SaveData::saveData.currencyList[currData->id + 1].unlocked) { // if currency and next are unlocked
				UcurrencyConverterBox* upgradeBox = new UcurrencyConverterBox(this, currData, currSaveData);
				if (upgradeBox->buyButton)
					upgradeBox->buyButton->setParent(upgradeHolder);
				std::cout << "adding child:" << std::endl;
				upgradeHolder->addChild(upgradeBox, upgradeBox->getSize().y);
			}
		}
	}
	setupLocs();
}