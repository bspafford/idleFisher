#include "AFmoreInfoUI.h"

#include <iostream>

#include "main.h"
#include "shortNumbers.h"
#include "AautoFisher.h"
#include "math.h"

// widgets
#include "text.h"
#include "button.h"

AFmoreInfoUI::AFmoreInfoUI(AautoFisher* autoFisher) {
	this->autoFisher = autoFisher;

	background = new Fimg("./images/autoFisher/moreUI/UI.png");

	fisherNum = new text("#" + std::to_string(autoFisher->autoFisherNum + 1), "afScreen", {0, 0}, false, false, textAlign::right);

	levelText = new text("123", "afScreen", { 100, 0 }, false, false, textAlign::right);

	fullnessText = new text("42/122", "afScreen", { 0, 0 }, false, false, textAlign::center);

	closeButton = new Ubutton(this, "./images/autoFisher/moreUI/xButton.png", 11, 11, 1, { 0, 0 }, false, false);
	closeButton->addCallback(this, &AFmoreInfoUI::closeUI);

	fpsText = new text(shortNumbers::convert2Short(autoFisher->calcFPS()), "afScreen", { 0, 0 });

	mpsText = new text("123mps", "afScreen", {0, 0});

	levelBar = new Fimg("./images/autoFisher/moreUI/level/level1.png", { 0, 0 });
	fullnessBar = new Fimg("./images/autoFisher/moreUI/fullness/fullness1.png", { 0, 0 });

	setupLocs();
}

AFmoreInfoUI::~AFmoreInfoUI() {
	delete background;
	delete fullnessBar;
	delete levelBar;
	delete fisherNum;
	delete levelText;
	delete fullnessText;
	delete fpsText;
	delete mpsText;
	delete closeButton;
	background = nullptr;
	fullnessBar = nullptr;
	levelBar = nullptr;
	fisherNum = nullptr;
	levelText = nullptr;
	fullnessText = nullptr;
	fpsText = nullptr;
	mpsText = nullptr;
	closeButton = nullptr;
}

void AFmoreInfoUI::setupLocs() {
	background->loc = { Main::screenWidth / 2 - background->w / 2 * stuff::pixelSize,  Main::screenHeight / 2 - background->h / 2 * stuff::pixelSize };
	fisherNum->setLoc({ Main::screenWidth / 2.f + 54 * stuff::pixelSize,  Main::screenHeight / 2.f - 79 * stuff::pixelSize });
	levelText->setLoc({Main::screenWidth / 2.f + 54 * stuff::pixelSize,  Main::screenHeight / 2.f - 46 * stuff::pixelSize});
	fullnessText->setLoc({ Main::screenWidth / 2.f,  Main::screenHeight / 2.f - 12 * stuff::pixelSize });
	if (closeButton)
		closeButton->setLoc({ Main::screenWidth / 2.f + 72 * stuff::pixelSize,  Main::screenHeight / 2.f - 108 * stuff::pixelSize });
	if (fpsText)
		fpsText->setLoc({ Main::screenWidth / 2.f - 54 * stuff::pixelSize,  Main::screenHeight / 2.f + 22 * stuff::pixelSize });
	if (mpsText)
		mpsText->setLoc({ Main::screenWidth / 2.f - 54 * stuff::pixelSize,  Main::screenHeight / 2.f + 42 * stuff::pixelSize });
	fullnessBar->loc = background->loc;
	levelBar->loc = background->loc;
}

void AFmoreInfoUI::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	background->draw(renderer, false);

	fisherNum->draw(renderer);
	levelText->draw(renderer);
	fullnessText->draw(renderer);
	fpsText->draw(renderer);
	mpsText->draw(renderer);
	fullnessBar->draw(renderer, false);
	levelBar->draw(renderer, false);

	closeButton->draw(renderer);
}

void AFmoreInfoUI::updateUI() {
	fpsText->setText(shortNumbers::convert2Short(autoFisher->calcFPS(), true) + "fps");
	mpsText->setText(shortNumbers::convert2Short(autoFisher->calcMPS(), true) + "mps");

	double level = *autoFisher->level;
	double maxLevel = autoFisher->maxLevel;
	int levelPercent = int(round(level / maxLevel * 100));
	levelText->setText(std::to_string(*autoFisher->level));
	levelBar->setImage("./images/autoFisher/moreUI/level/level" + std::to_string(levelPercent + 1) + ".png");

	double fullness = autoFisher->calcCurrencyHeld();
	double maxFullness = autoFisher->maxCurrency;
	int fullnessPercent = int(round(fullness / maxFullness * 100));
	fullnessText->setText(shortNumbers::convert2Short(fullness) + "/" + shortNumbers::convert2Short(maxFullness));
	fullnessBar->setImage("./images/autoFisher/moreUI/fullness/fullness" + std::to_string(fullnessPercent + 1) + ".png");
}

// kinda like a call back function
void AFmoreInfoUI::addToViewport(bool override) {
	__super::addToViewport(override);

	updateUI();
}

void AFmoreInfoUI::closeUI() {
	// because when the button disapears it doesn't remove hover num
	Main::hoverNum = 0;
	removeFromViewport();
}