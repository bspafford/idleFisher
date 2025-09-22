#include "autoFisherUI.h"

#include <string>

#include "AautoFisher.h"
#include "shortNumbers.h"
#include "math.h" 

#include "AFmoreInfoUI.h"
#include "animation.h"
#include "button.h"
#include "text.h"

autoFisherUI::autoFisherUI(AautoFisher* autoFisherRef, vector loc) {
	autoFisher = autoFisherRef;

	UILoc = loc + vector{ -12, 47 };

	// button
	upgradeButton = new Ubutton(NULL, "autoFisher/UI/button.png", 36, 66, 4, UILoc, true, true);
	upgradeButton->addCallback(this, &autoFisherUI::upgrade);
	upgradeButton->buttonAnim->addFrameCallback(this, &autoFisherUI::moveButtonText);

	// multipliers
	multi1 = new Ubutton(NULL, "autoFisher/UI/multipliers/multi1.png", 36, 66, 3, UILoc, true, true);
	multi10 = new Ubutton(NULL, "autoFisher/UI/multipliers/multi10.png", 36, 66, 3, UILoc, true, true);
	multiMax = new Ubutton(NULL, "autoFisher/UI/multipliers/multiMax.png", 36, 66, 3, UILoc, true, true);
	multi1->addCallback(this, &autoFisherUI::setMulti1);
	multi10->addCallback(this, &autoFisherUI::setMulti10);
	multiMax->addCallback(this, &autoFisherUI::setMultiMax);

	pressTest = new Image("./images/autoFisher/UI/multipliers/multi3.png", UILoc, true);

	menuButton = new Ubutton(NULL, "autoFisher/UI/menu.png", 5, 10, 1, UILoc + vector{ 0, 37 }, true, false);
	menuButton->addCallback(this, &autoFisherUI::openMenu);


	// level
	level = new Image("./images/autoFisher/UI/level1.png", UILoc, true);

	levelText = new text(std::to_string(*autoFisher->level), "tall", UILoc + vector{31, 66}, true, true, textAlign::right);
	maxText = new text(" ", "tall", UILoc + vector{31, 66}, true, true);
	maxText->setTextColor(124, 127, 85);
	buttonTextLoc = UILoc + vector{ 34, 23 };
	buttonText = new text(shortNumbers::convert2Short(autoFisher->getUpgradeCost()[1]), "normal", buttonTextLoc, true, true, textAlign::right);

	std::string levelBarPath = "./images/autoFisher/UI/level";
	for (int i = 0; i < 21; i++)
		levelBarImgs.push_back(levelBarPath + std::to_string(i + 1) + ".png");

	std::unordered_map<std::string, animDataStruct> openData;
	openData.insert({ "open", {{0, 0}, {3, 0}, .1, false} });
	openData.insert({ "close", {{0, 1}, {3, 1}, .1, false} });
	openAnimation = new animation("autoFisher/UI/openUI/openUI.png", 36, 66, openData, true, UILoc);
	openAnimation->setAnimation("open");
}

autoFisherUI::~autoFisherUI() {
	delete upgradeButton;
	delete multi1;
	delete multi10;
	delete multiMax;
	delete menuButton;
	delete level;
	delete levelText;
	delete maxText;
	delete buttonText;
	delete openAnimation;
	upgradeButton = nullptr;
	multi1 = nullptr;
	multi10 = nullptr;
	multiMax = nullptr;
	menuButton = nullptr;
	level = nullptr;
	levelText = nullptr;
	maxText = nullptr;
	buttonText = nullptr;
	openAnimation = nullptr;
}

void autoFisherUI::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	if (closing || opening) {
		openAnimation->draw(shaderProgram);
	} else {
		if (upgradeButton)
			upgradeButton->draw(shaderProgram);
		level->draw(shaderProgram);


		if (pressTest && autoFisher->multiplier == 1)
			pressTest->draw(shaderProgram);
		if (multiMax && autoFisher->multiplier != int(INFINITY))
			multiMax->draw(shaderProgram);
		if (multi10 && autoFisher->multiplier != 10)
			multi10->draw(shaderProgram);
		else if (pressTest && autoFisher->multiplier == 10)
			pressTest->draw(shaderProgram);
		if (multi1 && autoFisher->multiplier != 1)
			multi1->draw(shaderProgram);
		if (pressTest && autoFisher->multiplier == int(INFINITY))
			pressTest->draw(shaderProgram);
		
			
		if (menuButton)
			menuButton->draw(shaderProgram);

		// text
		levelText->draw(shaderProgram);
		if (*autoFisher->level < autoFisher->maxLevel)
			maxText->draw(shaderProgram);
		buttonText->draw(shaderProgram);
	}
}

void autoFisherUI::upgrade() {
	if (!visible)
		return;

	autoFisher->upgrade();

	updateUI();
}

void autoFisherUI::moveButtonText(int frame) {
	std::cout << "frame: " << frame << std::endl;
	std::vector<vector> delta = std::vector<vector>{ {0, 0}, { -3, -3 }, {1, 1}, {3, 2}, {-1, 0} };
	buttonText->setLoc(buttonText->loc + delta[frame]);

	if (frame == 0)
		buttonText->setLoc(buttonTextLoc);
}

void autoFisherUI::updateUI() {
	if (!visible)
		return;

	std::vector<double> upgradeCost = autoFisher->getUpgradeCost();

	// if has enough currency to upgrade it
	if (upgradeButton)
		upgradeButton->enable(upgradeCost[1] <= SaveData::saveData.currencyList[autoFisher->currencyId].numOwned);


	// update level num
	levelText->setText(std::to_string(*autoFisher->level));
	maxText->setText("+" + std::to_string(int(upgradeCost[0])));

	maxText->setLoc(levelText->loc - vector{ 0, maxText->getSize().y / stuff::pixelSize + 4 });

	if (*autoFisher->level < autoFisher->maxLevel)
		buttonText->setText(shortNumbers::convert2Short(upgradeCost[1]));
	else {
		buttonText->setText("max");
		upgradeButton->enable(false);
	}

	// based on the level up the level bar
	float levelIndex = floor((float(*autoFisher->level) / float(autoFisher->maxLevel)) * ((int)levelBarImgs.size() - 1));
	level = new Image(levelBarImgs[levelIndex], UILoc, true);
}

void autoFisherUI::openMenu() {
	autoFisher->afMoreInfoUI->addToViewport(true);
}

void autoFisherUI::openUI() {
	if (closing || opening)
		return;

	if (visible) {
		closeUI();
		return;
	}

	visible = true;
	updateUI();

	// do open animation
	closing = false;
	openAnimation->stop();
	openAnimation->setAnimation("open");
	openAnimation->addFinishedCallback(this, &autoFisherUI::openedUI);
	openAnimation->start();
	opening = true;
}

void autoFisherUI::openedUI() {
	opening = false;
}

void autoFisherUI::closeUI() {
	opening = false;
	openAnimation->stop();
	openAnimation->setAnimation("close");
	openAnimation->addFinishedCallback(this, &autoFisherUI::closedUI);
	openAnimation->start();
	closing = true;
}

void autoFisherUI::closedUI() {
	visible = false;
	closing = false;
}

void autoFisherUI::setMultiplier(int multiplier) {
	autoFisher->multiplier = multiplier;

	std::string path = "./images/autoFisher/UI/multipliers/multi";

	std::string mult = "max";
	if (autoFisher->multiplier == 1) {
		mult = "1";
		pressTest->setImage(path + "3.png");
	} else if (autoFisher->multiplier == 10) {
		mult = "10";
		pressTest->setImage(path + "12.png");
	} else
		pressTest->setImage(path + "Max3.png");


	updateUI();
}

void autoFisherUI::setMulti1() {
	setMultiplier(1);
	multi1->buttonAnim->setAnimation("click");
}
void autoFisherUI::setMulti10() {
	setMultiplier(10);
}
void autoFisherUI::setMultiMax() {
	setMultiplier(INFINITY);
}