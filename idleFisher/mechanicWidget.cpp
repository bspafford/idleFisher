#include "mechanicWidget.h"

#include "main.h"
#include "npc.h"
#include "currencyWidget.h"
#include "shortNumbers.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"
#include "progressBar.h"

#include "worlds.h"
#include "fishTransporter.h"

UmechanicWidget::UmechanicWidget(npc* parent) {
	this->parent = parent;

	int id = math::getWorldIndexFromName(Main::currWorld);
	saveMechanicStruct = &SaveData::saveData.mechanicStruct[id];
	mechanicStruct = &SaveData::data.mechanicStruct[id];

	closeButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &UmechanicWidget::removeFromViewport);
	npcImg = new Image("./images/widget/npcButtons/mechanic.png", { 0, 0 }, false);

	name = new text(" ", "biggerStraight", { 0,0 });
	description = new text(" ", "straight", { 0,0 });
	nameHolder = new verticalBox();
	nameHolder->addChild(name, 8 * stuff::pixelSize);
	nameHolder->addChild(description, 4 * stuff::pixelSize);

	npcBackground = new Image("./images/widget/npcBackground.png", { 0, 0 }, false);
	infoBackground = new Image("./images/widget/infoBackground.png", { 0, 0 }, false);
	upgradeBackground = new Image("./images/widget/upgradeBackground.png", { 0, 0 }, false);

	// fish transporter
	buyFishTransporterButton = new Ubutton(this, "widget/button.png", 27, 13, 2, { 0, 0 }, false, false);
	buyFishTransporterButton->addCallback(this, &UmechanicWidget::buyFishTransporter);
	buyFishTransporterText = new text("Buy Fish Transporter", "biggerStraight", { 0, 0 });
	buyFishTransporterPriceText = new text(shortNumbers::convert2Short(mechanicStruct->currencyNum), "straight", {0, 0});

	// bought screen
	fishTransporterName = new text("Fish Transporter", "biggerStraight", { 0, 0 });
	fishTransporterImg = new Image("./images/npc/fishTransporter/idleSE.png", { 0, 0 }, false);
	
	if (saveMechanicStruct->unlocked)
		fishTransporterImg->setColorMod(glm::vec4(1));
	else
		fishTransporterImg->setColorMod(glm::vec4(glm::vec3(0), 1.f));

	fishTransporterImg->w *= 2.5; // temp
	fishTransporterImg->h *= 2.5; // temp
	level = new text("0/100", "biggerStraight", { 0, 0 }, false, false, textAlign::right);
	levelProgress = new UprogressBar(false, 125, 7);
	maxHoldText = new text("Max Hold:", "straight", { 0, 0 });
	maxHoldValue = new text(shortNumbers::convert2Short(0), "straight", {0, 0}, false, false, textAlign::right);
	speedText = new text("Walk Speed:", "straight", { 0, 0 });
	speedValue = new text(shortNumbers::convert2Short(0), "straight", { 0, 0 }, false, false, textAlign::right);
	collectSpeedText = new text("Collect Speed:", "straight", { 0, 0 });
	collectSpeedValue = new text(shortNumbers::convert2Short(0), "straight", { 0, 0 }, false, false, textAlign::right);
	
	buyButton = new Ubutton(this, "widget/biggerButton.png", 31, 15, 2, { 0, 0 }, false, false);
	buyButton->addCallback(this, &UmechanicWidget::upgradeFishTransporter);
	//buyButtonText = new text("Upgrade", "straight", { 0, 0 });
	multi1x = new Ubutton(this, "widget/button.png", 27, 13, 2, { 0, 0 }, false, false);
	multi10x = new Ubutton(this, "widget/button.png", 27, 13, 2, { 0, 0 }, false, false);
	multiMax = new Ubutton(this, "widget/button.png", 27, 13, 2, { 0, 0 }, false, false);

	upgradePriceText = new text("0.00k", "straight", { 0, 0 }, false, false, textAlign::center);
	currencyIcon = new Image("./images/currency/coin" + std::to_string(id + 1) + ".png", {0, 0}, false);

	setup();
}

void UmechanicWidget::setup() {
	setupLocs();
}

UmechanicWidget::~UmechanicWidget() {
	delete closeButton;
	delete npcImg;
	delete name;
	delete description;
	delete nameHolder;
	delete upgradeBackground;
	delete infoBackground;
	delete npcBackground;
	delete fishTransporterName;
	delete fishTransporterImg;
	delete level;
	delete levelProgress;
	delete maxHoldText;
	delete maxHoldValue;
	delete speedText;
	delete speedValue;
	delete collectSpeedText;
	delete collectSpeedValue;
	delete buyButton;
	//delete buyButtonText;
	delete multi1x;
	delete multi10x;
	delete multiMax;
	closeButton = nullptr;
	npcImg = nullptr;
	name = nullptr;
	description = nullptr;
	nameHolder = nullptr;
	upgradeBackground = nullptr;
	infoBackground = nullptr;
	npcBackground = nullptr;
	fishTransporterName = nullptr;
	fishTransporterImg = nullptr;
	level = nullptr;
	levelProgress = nullptr;
	maxHoldText = nullptr;
	maxHoldValue = nullptr;
	speedText = nullptr;
	speedValue = nullptr;
	collectSpeedText = nullptr;
	collectSpeedValue = nullptr;
	buyButton = nullptr;
	//buyButtonText = nullptr;
	multi1x = nullptr;
	multi10x = nullptr;
	multiMax = nullptr;
}

void UmechanicWidget::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);

	if (!visible)
		return;

	upgradeBackground->draw(shaderProgram);
	npcBackground->draw(shaderProgram);
	if (infoBackground)
		infoBackground->draw(shaderProgram);

	npcImg->draw(shaderProgram);

	nameHolder->draw(shaderProgram);

	fishTransporterImg->draw(shaderProgram);
	// fish transporter
	if (!saveMechanicStruct->unlocked) {
		buyFishTransporterButton->draw(shaderProgram);
		buyFishTransporterText->draw(shaderProgram);
		buyFishTransporterPriceText->draw(shaderProgram);
	} else {
		fishTransporterName->draw(shaderProgram);
		level->draw(shaderProgram);
		levelProgress->draw(shaderProgram);
		maxHoldText->draw(shaderProgram);
		maxHoldValue->draw(shaderProgram);
		speedText->draw(shaderProgram);
		speedValue->draw(shaderProgram);
		collectSpeedText->draw(shaderProgram);
		collectSpeedValue->draw(shaderProgram);
		buyButton->draw(shaderProgram);
		//buyButtonText->draw(shaderProgram);
		//multi1x->draw(shaderProgram);
		//multi10x->draw(shaderProgram);
		//multiMax->draw(shaderProgram);

		upgradePriceText->draw(shaderProgram);
		currencyIcon->draw(shaderProgram);
	}

	closeButton->draw(shaderProgram);
}

void UmechanicWidget::setNameDescription(std::string nameString, std::string descriptionString) {
	name->setText(nameString);
	description->setText(descriptionString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name, name->getSize().y + stuff::pixelSize);
}

void UmechanicWidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { stuff::screenSize.x / 2, stuff::screenSize.y / 2 };
	vector topLeft = center - size / 2;
	npcBackground->setLoc(topLeft);
	if (infoBackground)
		infoBackground->setLoc(topLeft + vector{ 0, y });
	upgradeBackground->setLoc(topLeft + vector{x, 0});

	// crops the bottom part of the npc img
	//SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	//npcImg->h = 25;
	//npcImg->setSourceRect(source);

	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->setLoc(npcBackground->getLoc() + vector{npcBackgroundSize.x / 2, npcBackgroundSize.y} - vector{npcSize.x / 2, npcSize.y});

	if (closeButton) {
		vector closeButtonSize = closeButton->getSize();
		closeButton->setLoc({ float(upgradeBackground->getLoc().x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->getLoc().y - closeButtonSize.y / 2)});
	}

	if (infoBackground) {
		nameHolder->setLocAndSize({ float(infoBackground->getLoc().x) + 6 * stuff::pixelSize, float(infoBackground->getLoc().y) + 6 * stuff::pixelSize}, vector{float(infoBackground->w), float(infoBackground->h)} *stuff::pixelSize);
		name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
		description->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	}

	// bought fish transporter
	fishTransporterName->setLoc(upgradeBackground->getLoc() + vector{6, 6} *stuff::pixelSize);
	buyFishTransporterText->setLoc(fishTransporterName->getLoc());
	fishTransporterImg->setLoc(fishTransporterName->loc + vector{ 2 * stuff::pixelSize, fishTransporterName->getSize().y + 15 * stuff::pixelSize});

	float xStart = fishTransporterImg->getLoc().x + fishTransporterImg->getSize().x;
	float xEnd = upgradeBackground->getLoc().x + upgradeBackground->getSize().x - 6 * stuff::pixelSize;
	float xMid = (xEnd - xStart) / 2.f + xStart;
	if (buyFishTransporterButton) {
		//buyFishTransporterButton->setLoc(upgradeBackground->loc + upgradeBackground->getSize() / 2 - buyFishTransporterButton->getSize() / 2);
		// find mid between fish transporter and edge of shop
		buyFishTransporterButton->setLoc(vector{ xMid - buyFishTransporterButton->getSize().x / 2.f, upgradeBackground->getLoc().y + upgradeBackground->getSize().y / 2 - buyFishTransporterButton->getSize().y / 2});
		//buyFishTransporterPriceText->setLoc(upgradeBackground->loc + upgradeBackground->getSize() / 2 - buyFishTransporterPriceText->getSize() / 2);
		buyFishTransporterPriceText->setLoc({ xMid - buyFishTransporterPriceText->getSize().x / 2.f, upgradeBackground->getLoc().y + upgradeBackground->getSize().y / 2 - buyFishTransporterPriceText->getSize().y / 2});
	}

	level->setLoc(vector{ fishTransporterImg->getLoc().x + fishTransporterImg->getSize().x + 130 * stuff::pixelSize, upgradeBackground->getLoc().y + 15 * stuff::pixelSize});
	levelProgress->setLoc(level->getLoc() + vector{ -125, 9 } * stuff::pixelSize);
	maxHoldText->setLoc(levelProgress->getLoc() + vector{ 0, 20 * stuff::pixelSize });
	maxHoldValue->setLoc(vector{ upgradeBackground->getLoc().x + upgradeBackground->getSize().x - 5 * stuff::pixelSize, maxHoldText->getLoc().y});
	speedText->setLoc(maxHoldText->getLoc() + vector{ 0, 20 * stuff::pixelSize });
	speedValue->setLoc(maxHoldValue->getLoc() + vector{ 0, 20 * stuff::pixelSize });
	collectSpeedText->setLoc(speedText->getLoc() + vector{ 0, 20 * stuff::pixelSize });
	collectSpeedValue->setLoc(speedValue->getLoc() + vector{ 0, 20 * stuff::pixelSize });
	if (buyButton) {
		//buyButton->setLoc(upgradeBackground->loc + upgradeBackground->getSize() - buyButton->getSize() - vector{ 5, 5 } *stuff::pixelSize);
		buyButton->setLoc({ xMid - buyButton->getSize().x / 2.f, collectSpeedText->getLoc().y + 20 * stuff::pixelSize });
		//buyButtonText->setLoc(buyButton->getLoc() + buyButton->getSize() / 2 - buyButtonText->getSize() / 2);
		multiMax->setLoc(buyButton->getLoc() - vector{ buyButton->getSize().x + 3 * stuff::pixelSize, 0 });
		multi10x->setLoc(multiMax->getLoc() - vector{ multiMax->getSize().x, 0 });
		multi1x->setLoc(multi10x->getLoc() - vector{ multi10x->getSize().x, 0 });
		upgradePriceText->setLoc(buyButton->getLoc() + buyButton->getSize() / 2.f - vector{ 0, upgradePriceText->getSize().y / 2.f });
		currencyIcon->setLoc(buyButton->getLoc() + vector{ -currencyIcon->getSize().x - stuff::pixelSize * 2.f, buyButton->getSize().y / 2.f - currencyIcon->getSize().y / 2.f});
	}
}

void UmechanicWidget::buyFishTransporter() {
	int id = math::getWorldIndexFromName(Main::currWorld);
	if (SaveData::saveData.currencyList[id+1].numOwned >= mechanicStruct->currencyNum) {
		SaveData::saveData.currencyList[id+1].numOwned -= mechanicStruct->currencyNum;
		saveMechanicStruct->unlocked = true;
		Main::currencyWidget->updateList();

		// spawn the fish transproter
		std::cout << "spawning the fish transporter!" << std::endl;
		world::spawnFishTransporter();
		fishTransporterImg->setColorMod(glm::vec4(1));

		// update text
	}

	update();
}

void UmechanicWidget::update() {
	level->setText(std::to_string(saveMechanicStruct->level) + "/100");
	levelProgress->setPercent(saveMechanicStruct->level / 100.f);

	upgradePriceText->setText(shortNumbers::convert2Short(calcUpgradeCost()));

	if (world::fishTransporter) {
		maxHoldValue->setText(shortNumbers::convert2Short(world::fishTransporter->getMaxHoldNum()));
		speedValue->setText(shortNumbers::convert2Short(world::fishTransporter->getSpeed()));
		collectSpeedValue->setText(shortNumbers::convert2Short(world::fishTransporter->getCollectionSpeed(), true));
	}
}

void UmechanicWidget::upgradeFishTransporter() {
	if (saveMechanicStruct->level < 100) {
		// saveMechanicStruct->level++;
		if (world::fishTransporter)
			world::fishTransporter->upgrade(saveMechanicStruct);
		update();
	}
}

double UmechanicWidget::calcUpgradeCost() {
	return saveMechanicStruct->level * 100;
}