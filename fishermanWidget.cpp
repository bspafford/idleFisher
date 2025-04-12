#include "fishermanWidget.h"

#include "main.h"
#include "npc.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"

UfishermanWidget::UfishermanWidget(npc* parent) {
	this->parent = parent;

	closeButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &NPCwidget::removeFromViewport);
	npcImg = new Fimg("./images/widget/npcbuttons/fisherman.png", {100, 100});

	name = new text(" ", "biggerStraight", { 0,0 });
	buffText = new text(" ", "straight", { 0,0 });
	buffText->setTextColor(0, 255, 0);
	debuffText = new text(" ", "straight", { 0,0 });
	debuffText->setTextColor(255, 0, 0);
	nameHolder = new verticalBox();
	nameHolder->addChild(name, 8 * stuff::pixelSize);
	nameHolder->addChild(buffText, 7 * stuff::pixelSize);
	nameHolder->addChild(debuffText, 7 * stuff::pixelSize);

	upgradeBackground = new Fimg("./images/widget/upgradeBackground.png");
	infoBackground = new Fimg("./images/widget/infoBackground.png");
	npcBackground = new Fimg("./images/widget/npcBackground.png");

	selectedIcon = new Fimg("./images/widget/selectedFisherIcon.png");

	setup();
	//setupLocs();
}

UfishermanWidget::~UfishermanWidget() {
	for (int i = 0; i < upgradeHolderList.size(); i++)
		delete upgradeHolderList[i];
	upgradeHolderList.clear();
	delete closeButton;
	delete npcImg;
	delete name;
	delete buffText;
	delete debuffText;
	delete nameHolder;
	delete upgradeBackground;
	delete infoBackground;
	delete npcBackground;
	closeButton = nullptr;
	npcImg = nullptr;
	name = nullptr;
	buffText = nullptr;
	debuffText = nullptr;
	nameHolder = nullptr;
	upgradeBackground = nullptr;
	infoBackground = nullptr;
	npcBackground = nullptr;
}

void UfishermanWidget::setup() {
	// setup fishing rods
	UscrollBox* scrollBox = new UscrollBox();
	upgradeHolderList.push_back(scrollBox);
	for (int i = 0; i < SaveData::data.fishingRodData.size(); i++) {
		FfishingRodStruct* currData = &SaveData::data.fishingRodData[i];
		FsaveFishingRodStruct* currSaveData = &SaveData::saveData.fishingRodList[currData->id];

		UupgradeBox* upgradeBox = new UupgradeBox(this, currData, currSaveData);
		if (upgradeBox->buyButton)
			upgradeBox->buyButton->setParent(upgradeHolderList[0]);
		upgradeHolderList[0]->addChild(upgradeBox, upgradeBox->getSize().y);
	}

	// setup bait
	UscrollBox* scrollBox2 = new UscrollBox();
	scrollBox2->setVisibility(false);
	upgradeHolderList.push_back(scrollBox2);
	for (int i = 0; i < SaveData::data.baitData.size(); i++) {
		FbaitStruct* currData = &SaveData::data.baitData[i];
		FsaveBaitStruct* currSaveData = &SaveData::saveData.baitList[currData->id];

		UupgradeBox* upgradeBox = new UupgradeBox(this, currData, currSaveData);
		if (upgradeBox->buyButton)
			upgradeBox->buyButton->setParent(upgradeHolderList[1]);
		upgradeHolderList[1]->addChild(upgradeBox, upgradeBox->getSize().y);
	}

	
	// set view box images
	Ubutton* button = new Ubutton(this, "widget/thumbnails/fishingRodThumbnail1.png", 16, 16, 1, { 0, 0 }, false, false);
	buttonList.push_back(button);
	Ubutton* button2 = new Ubutton(this, "widget/thumbnails/worm.png", 16, 16, 1, { 0, 0 }, false, false);
	buttonList.push_back(button2);

	buttonList[0]->addCallback(this, &UfishermanWidget::setPage1);
	buttonList[1]->addCallback(this, &UfishermanWidget::setPage2);

	setupLocs();
}

void UfishermanWidget::showUpgradePage(int index) {
	selectedPageIndex = index;
	for (int i = 0; i < upgradeHolderList.size(); i++)
		upgradeHolderList[i]->setVisibility(i == index);
}

void UfishermanWidget::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	if (!visible)
		return;

	vector selectedPos = buttonList[selectedPageIndex]->getLoc();
	selectedIcon->loc = { selectedPos.x - stuff::pixelSize, selectedPos.y - stuff::pixelSize };
	selectedIcon->draw(renderer, false);

	upgradeBackground->draw(renderer, false);
	npcBackground->draw(renderer, false);
	infoBackground->draw(renderer, false);

	npcImg->draw(renderer, false);

	nameHolder->draw(renderer);

	for (int i = 0; i < upgradeHolderList.size(); i++) {
		upgradeHolderList[i]->draw(renderer);
		buttonList[i]->draw(renderer);
	}

	closeButton->draw(renderer);
}

void UfishermanWidget::setNameDescription(std::string nameString, std::string buffString, std::string debuffString) {
	name->setText(nameString);
	buffText->setText(buffString);
	debuffText->setText(debuffString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name, name->getSize().y + stuff::pixelSize);
}

void UfishermanWidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { Main::screenWidth / 2, Main::screenHeight / 2 };
	vector topLeft = center - size / 2;
	npcBackground->loc = topLeft;
	infoBackground->loc = topLeft + vector{ 0, y };
	upgradeBackground->loc = topLeft + vector{ x, 0 };

	// crops the bottom part of the npc img
	//SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	//npcImg->h = 25;
	//npcImg->setSourceRect(source);

	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->loc = npcBackground->loc + vector{ npcBackgroundSize.x / 2, npcBackgroundSize.y } - vector{ npcSize.x / 2, npcSize.y } - vector{ 0, 3 * stuff::pixelSize };

	for (int i = 0; i < upgradeHolderList.size(); i++) {
		upgradeHolderList[i]->setLocAndSize(upgradeBackground->loc, upgradeBackground->getSize() - vector{0, 6 * stuff::pixelSize});
		upgradeHolderList[i]->ogLoc = upgradeBackground->loc + vector{ 4, 3 } *stuff::pixelSize;

		vector worldButtonSize = buttonList[i]->getSize();
		buttonList[i]->setLoc(upgradeBackground->loc + vector{ (worldButtonSize.x + stuff::pixelSize) * i + stuff::pixelSize, -worldButtonSize.y - stuff::pixelSize });
	}

	vector closeButtonSize = closeButton->getSize();
	closeButton->setLoc({ float(upgradeBackground->loc.x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->loc.y - closeButtonSize.y / 2) });

	nameHolder->setLocAndSize({ float(infoBackground->loc.x) + 6 * stuff::pixelSize, float(infoBackground->loc.y) + 6 * stuff::pixelSize }, vector{ float(infoBackground->w), float(infoBackground->h) } *stuff::pixelSize);
	name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	buffText->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	debuffText->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
}