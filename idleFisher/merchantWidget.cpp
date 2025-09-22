#include "merchantWidget.h"

#include "main.h"
#include "npc.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"

UmerchantWidget::UmerchantWidget(npc* parent) {
	this->parent = parent;

	closeButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	if (closeButton)
		closeButton->addCallback<widget>(this, &NPCwidget::removeFromViewport);
	npcImg = new Image("./images/widget/npcButtons/merchant.png", {100, 100}, false);
	npcImg->w *= 1;
	npcImg->h *= 1;

	name = new text(" ", "biggerStraight", { 0,0 });
	description = new text(" ", "straight", { 0,0 });
	nameHolder = new verticalBox();
	if (nameHolder) {
		nameHolder->addChild(name, 8 * stuff::pixelSize);
		nameHolder->addChild(description, 4 * stuff::pixelSize);
	}

	upgradeBackground = new Image("./images/widget/upgradeBackground.png", { 0, 0 }, false);
	infoBackground = new Image("./images/widget/infoBackground.png", { 0, 0 }, false);
	npcBackground = new Image("./images/widget/npcBackground.png", { 0, 0 }, false);

	selectedWorldIcon = new Image("./images/widget/selectedWorldIcon.png", { 0, 0 }, false);

	setup();
	//setupLocs();
}

UmerchantWidget::~UmerchantWidget() {
	for (int i = 0; i < upgradeHolderList.size(); i++)
		delete upgradeHolderList[i];
	upgradeHolderList.clear();
	delete closeButton;
	delete npcImg;
	delete name;
	delete description;
	delete nameHolder;
	delete upgradeBackground;
	delete infoBackground;
	delete npcBackground;
	closeButton = nullptr;
	npcImg = nullptr;
	name = nullptr;
	description = nullptr;
	nameHolder = nullptr;
	upgradeBackground = nullptr;
	infoBackground = nullptr;
	npcBackground = nullptr;
}

void UmerchantWidget::setup() {
	// see if world name is different from last
		// if it is then add new scroll box to list
	// else
		// add the upgrade box to the last index of the list
	std::string lastWorldName = "";
	int worldIndex = 1;
	for (int i = 0; i < SaveData::data.upgradeData.size(); i++) {
		FupgradeStruct* currData = &SaveData::data.upgradeData[i];
		FsaveUpgradeStruct* currSaveData = &SaveData::saveData.upgradeList[currData->id];

		if (currData->levelName != lastWorldName) {
			// for every new world add a button to the list
			
			Ubutton* button = new Ubutton(this, "currency/coin" + std::to_string(worldIndex) + ".png", 12, 12, 1, { 0, 0 }, false, false);
			worldButtonList.push_back(button);

			UscrollBox* scrollBox = new UscrollBox();
			scrollBox->setVisibility(false);
			upgradeHolderList.push_back(scrollBox);
			lastWorldName = currData->levelName;
			
			worldIndex++;
		}
		
		UupgradeBox* upgradeBox = new UupgradeBox(this, currData, currSaveData);
		if (upgradeBox->buyButton)
			upgradeBox->buyButton->setParent(upgradeHolderList[upgradeHolderList.size() - 1]);
		upgradeHolderList[upgradeHolderList.size() - 1]->addChild(upgradeBox, upgradeBox->getSize().y);
		
	}
	upgradeHolderList[0]->setVisibility(true);

	if (worldButtonList.size() >= 10) {
		worldButtonList[0]->addCallback(this, &UmerchantWidget::setPage1);
		worldButtonList[1]->addCallback(this, &UmerchantWidget::setPage2);
		worldButtonList[2]->addCallback(this, &UmerchantWidget::setPage3);
		worldButtonList[3]->addCallback(this, &UmerchantWidget::setPage4);
		worldButtonList[4]->addCallback(this, &UmerchantWidget::setPage5);
		worldButtonList[5]->addCallback(this, &UmerchantWidget::setPage6);
		worldButtonList[6]->addCallback(this, &UmerchantWidget::setPage7);
		worldButtonList[7]->addCallback(this, &UmerchantWidget::setPage8);
		worldButtonList[8]->addCallback(this, &UmerchantWidget::setPage9);
		worldButtonList[9]->addCallback(this, &UmerchantWidget::setPage10);
	}

	setupLocs();
}

void UmerchantWidget::showUpgradePage(int index) {
	selectedPageIndex = index;
	for (int i = 0; i < upgradeHolderList.size(); i++)
		upgradeHolderList[i]->setVisibility(i == index);
}

void UmerchantWidget::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);

	if (!visible)
		return;

	vector selectedPos = worldButtonList[selectedPageIndex]->getLoc();
	selectedWorldIcon->setLoc({ selectedPos.x - stuff::pixelSize, selectedPos.y - stuff::pixelSize });
	selectedWorldIcon->draw(shaderProgram);

	upgradeBackground->draw(shaderProgram);
	npcBackground->draw(shaderProgram);
	infoBackground->draw(shaderProgram);

	npcImg->draw(shaderProgram);

	nameHolder->draw(shaderProgram);

	for (int i = 0; i < upgradeHolderList.size(); i++) {
		if (!SaveData::saveData.worldList[i].unlocked)
			break;
		upgradeHolderList[i]->draw(shaderProgram);
		worldButtonList[i]->draw(shaderProgram);
	}

	closeButton->draw(shaderProgram);
}

void UmerchantWidget::setNameDescription(std::string nameString, std::string descriptionString) {
	name->setText(nameString);
	description->setText(descriptionString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name, name->getSize().y + stuff::pixelSize);
}

void UmerchantWidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { stuff::screenSize.x / 2, stuff::screenSize.y / 2 };
	vector topLeft = center - size / 2;
	npcBackground->setLoc(topLeft);
	infoBackground->setLoc(topLeft + vector{ 0, y });
	upgradeBackground->setLoc(topLeft + vector{ x, 0 });

	/*
	// crops the bottom part of the npc img
	SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	npcImg->h = 25;
	npcImg->setSourceRect(source);

	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->loc = npcBackground->loc + vector{ npcBackgroundSize.x / 2, npcBackgroundSize.y } - vector{ npcSize.x / 2, npcSize.y } - vector{ 0, 3 * stuff::pixelSize };
	*/
	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->setLoc(npcBackground->getLoc() + vector{npcBackgroundSize.x / 2, npcBackgroundSize.y} - vector{npcSize.x / 2, npcSize.y} - vector{0, 3 * stuff::pixelSize});

	vector upgradeHolderLoc = upgradeBackground->getLoc() + vector{ 4, 3 } * stuff::pixelSize;
	for (int i = 0; i < upgradeHolderList.size(); i++) {
		upgradeHolderList[i]->setLocAndSize(upgradeHolderLoc, upgradeBackground->getSize() - vector{0, 6} * stuff::pixelSize);
		upgradeHolderList[i]->ogLoc = upgradeHolderLoc;

		if (worldButtonList.size() > i) {
			vector worldButtonSize = worldButtonList[i]->getSize();
			worldButtonList[i]->setLoc(upgradeBackground->getLoc() + vector{ (worldButtonSize.x + stuff::pixelSize) * i + stuff::pixelSize, -worldButtonSize.y - stuff::pixelSize });
		}
	}

	if (closeButton) {
		vector closeButtonSize = closeButton->getSize();
		closeButton->setLoc({ float(upgradeBackground->getLoc().x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->getLoc().y - closeButtonSize.y / 2) });
	}

	nameHolder->setLocAndSize({ float(infoBackground->getLoc().x) + 6 * stuff::pixelSize, float(infoBackground->getLoc().y) + 6 * stuff::pixelSize }, vector{ float(infoBackground->w), float(infoBackground->h) } *stuff::pixelSize);
	name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	description->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
}