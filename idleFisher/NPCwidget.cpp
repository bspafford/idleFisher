#include "NPCwidget.h"

#include "main.h"
#include "npc.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"

#include "image.h"

NPCwidget::NPCwidget(npc* parent, std::string npcName) {
	this->parent = parent;

	upgradeHolder = new UscrollBox();
	
	closeButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &NPCwidget::removeFromViewport);
	
	if (npcName != "fishGod")
		npcImg = new Image("./images/widget/npcButtons/" + npcName + ".png", {0, 0}, false);

	name = new text(" ", "biggerStraightDark", { 0,0 });
	description = new text(" ", "straightDark", { 0,0 });
	nameHolder = new verticalBox();
	nameHolder->addChild(name, 8 * stuff::pixelSize);
	nameHolder->addChild(description, 4 * stuff::pixelSize);

	npcBackground = new Image("./images/widget/npcBackground.png", { 0, 0 }, false);
	infoBackground = new Image("./images/widget/infoBackground.png", { 0, 0 }, false);
	upgradeBackground = new Image("./images/widget/upgradeBackground.png", { 0, 0 }, false);

	setupLocs();
}

NPCwidget::~NPCwidget() {
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

void NPCwidget::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);

	if (!visible)
		return;

	upgradeBackground->draw(shaderProgram);
	npcBackground->draw(shaderProgram);
	if (infoBackground)
		infoBackground->draw(shaderProgram);

	if (npcImg)
		npcImg->draw(shaderProgram);

	nameHolder->draw(shaderProgram);
	upgradeHolder->draw(shaderProgram);

	if (closeButton)
		closeButton->draw(shaderProgram);
}

void NPCwidget::setNameDescription(std::string nameString, std::string descriptionString) {
	name->setText(nameString);
	description->setText(descriptionString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name, name->getSize().y + stuff::pixelSize);
}

void NPCwidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { stuff::screenSize.x / 2, stuff::screenSize.y / 2 };
	vector topLeft = center - size / 2;
	npcBackground->setLoc(topLeft);
	if (infoBackground)
		infoBackground->setLoc(topLeft + vector{ 0, y });
	upgradeBackground->setLoc(topLeft + vector{ x, 0 });

	// crops the bottom part of the npc img
	//SDL_Rect* source = new SDL_Rect{ 0, 0, npcImg->w, 25 };
	//npcImg->h = 25;
	//npcImg->setSourceRect(source);


	if (npcImg) {
		vector npcBackgroundSize = npcBackground->getSize();
		vector npcSize = npcImg->getSize();
		npcImg->setLoc(npcBackground->getLoc() + vector{ npcBackgroundSize.x / 2, npcBackgroundSize.y } - vector{ npcSize.x / 2, npcSize.y } - vector{ 0, 1 * stuff::pixelSize });
	}


	vector upgradeHolderPos = (upgradeBackground->getLoc() + vector{ 4, 3 } * stuff::pixelSize).round();
	upgradeHolder->setLocAndSize(upgradeHolderPos, vector{float(upgradeBackground->w), float(upgradeBackground->h - 6)} *stuff::pixelSize);
	upgradeHolder->ogLoc = upgradeHolderPos;

	if (closeButton) {
		vector closeButtonSize = closeButton->getSize();
		closeButton->setLoc({ float(upgradeBackground->getLoc().x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->getLoc().y - closeButtonSize.y / 2)});
	}

	if (infoBackground) {
		nameHolder->setLocAndSize({ float(infoBackground->getLoc().x) + 6 * stuff::pixelSize, float(infoBackground->getLoc().y) + 6 * stuff::pixelSize}, vector{float(infoBackground->w), float(infoBackground->h)} *stuff::pixelSize);
		name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
		description->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	}
}