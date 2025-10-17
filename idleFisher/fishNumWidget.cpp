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

	thumbnail = std::make_unique<Image>(fish->thumbnail, vector{ 0, 0 }, false);
	
	if (fishQuality == 1)
		star = std::make_unique<Image>("./images/widget/tinyBronzeStar.png", vector{ 0, 0 }, false);
	else if (fishQuality == 2)
		star = std::make_unique<Image>("./images/widget/tinySilverStar.png", vector{ 0, 0 }, false);
	else if (fishQuality == 3)
		star = std::make_unique<Image>("./images/widget/tinyGoldStar.png", vector{ 0, 0 }, false);

	setupText();
}

void UfishNumWidget::setup(FcurrencyStruct* currency, double num) {
	thumbnail = std::make_unique<Image>(currency->thumbnail, vector{ 0, 0 }, false);

	currencyNum = num;
	setupText();
}

UfishNumWidget::~UfishNumWidget() {

}

void UfishNumWidget::setupText() {
	if (saveFish)
		numText = std::make_unique<text>(shortNumbers::convert2Short(saveFish->numOwned[fishQuality]), "straight", vector{ 0, 0 });
	else
		numText = std::make_unique<text>(shortNumbers::convert2Short(currencyNum), "straight", vector{ 0, 0 });

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
	loc = loc.round();
	__super::setLoc(loc);

	if (thumbnail)
		thumbnail->setLoc(loc);
	
	if (numText && thumbnail)
		numText->setLoc((loc + vector{ 18 * stuff::pixelSize, thumbnail->getSize().y / 2.f - numText->getSize().y / 2.f}).round());

	if (fishQuality != 0 && star) {
		star->setLoc((loc + vector{ 10, 10 } * stuff::pixelSize).round());
	}
}
