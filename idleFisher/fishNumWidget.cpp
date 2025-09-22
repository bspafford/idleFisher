#include "fishNumWidget.h"

#include <iostream>

#include "stuff.h"
#include "shortNumbers.h"
#include "saveData.h"
#include "image.h"
#include "text.h"

// setup / update
void UfishNumWidget::setup(FfishData* fish, FsaveFishData* saveFish, int fishQuality) {
	this->fish = fish;
	this->saveFish = saveFish;
	this->fishQuality = fishQuality;

	thumbnail = new Image(fish->thumbnail, { 0, 0 }, false);
	
	if (fishQuality == 1)
		star = new Image("./images/widget/tinyBronzeStar.png", { 0, 0 }, false);
	else if (fishQuality == 2)
		star = new Image("./images/widget/tinySilverStar.png", { 0, 0 }, false);
	else if (fishQuality == 3)
		star = new Image("./images/widget/tinyGoldStar.png", { 0, 0 }, false);

	setupText();
}

void UfishNumWidget::setup(FcurrencyStruct* currency, double num) {
	thumbnail = new Image(currency->thumbnail, { 0, 0 }, false);

	currencyNum = num;
	setupText();
}

UfishNumWidget::~UfishNumWidget() {
	delete star;
	delete thumbnail;
	delete numText;
	star = nullptr;
	thumbnail = nullptr;
	numText = nullptr;
}

void UfishNumWidget::setupText() {
	if (saveFish)
		numText = new text(shortNumbers::convert2Short(saveFish->numOwned[fishQuality]), "straight", { 0, 0 });
	else
		numText = new text(shortNumbers::convert2Short(currencyNum), "straight", { 0, 0 });

	if (thumbnail)
		size = thumbnail->getSize() + vector{ 30, 1 } * stuff::pixelSize;

	/*
	TTF_Font* Sans = TTF_OpenFont("./fonts/pixelFont.ttf", 5 * stuff::pixelSize);
	SDL_Color White = { 255, 255, 255 };

	if (saveFish)
		surfaceText = TTF_RenderText_Solid(Sans, shortNumbers::convert2Short(saveFish->numOwned[fishQuality]).c_str(), White);
	else
		surfaceText = TTF_RenderText_Solid(Sans, shortNumbers::convert2Short(currencyNum).c_str(), White);

	text = SDL_CreateTextureFromSurface(shaderProgram, surfaceText);

	SDL_QueryTexture(text, NULL, NULL, &textW, &textH);

	TTF_CloseFont(Sans);
	*/
}

void UfishNumWidget::draw(Shader* shaderProgram) { // , float x, float y
	if (thumbnail)
		thumbnail->draw(shaderProgram);
	if (numText)
		numText->draw(shaderProgram);

	if (star && fishQuality != 0) {
		star->draw(shaderProgram);
	}
}

void UfishNumWidget::setLoc(vector loc) {
	__super::setLoc(loc);

	if (thumbnail)
		thumbnail->setLoc(loc);
	
	if (numText && thumbnail)
		numText->setLoc((loc + vector{ 18 * stuff::pixelSize, thumbnail->getSize().y / 2.f - numText->getSize().y / 2.f}).round());

	if (fishQuality != 0 && star) {
		star->setLoc((loc + vector{ 10, 10 } * stuff::pixelSize).round());
	}
}
