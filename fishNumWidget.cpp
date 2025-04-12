#include "fishNumWidget.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

#include "stuff.h"
#include "shortNumbers.h"
#include "saveData.h"

// setup / update
void UfishNumWidget::setup(SDL_Renderer* renderer, FfishData* fish, FsaveFishData* saveFish, int fishQuality) {
	texture = IMG_LoadTexture(renderer, fish->thumbnail.c_str());
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	this->fish = fish;
	this->saveFish = saveFish;
	this->fishQuality = fishQuality;

	if (fishQuality == 1)
		star = new Fimg("./images/widget/tinyBronzeStar.png");
	else if (fishQuality == 2)
		star = new Fimg("./images/widget/tinySilverStar.png");
	else if (fishQuality == 3)
		star = new Fimg("./images/widget/tinyGoldStar.png");

	setupText(renderer);

}

UfishNumWidget::~UfishNumWidget() {
	if (text)
		SDL_DestroyTexture(text);
	if (texture)
		SDL_DestroyTexture(texture);
	SDL_FreeSurface(surfaceText);

	delete star;
}

void UfishNumWidget::setup(SDL_Renderer* renderer, FcurrencyStruct* currency, double num) {
	texture = IMG_LoadTexture(renderer, currency->thumbnail.c_str());
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	widgetSize = vector{ float(w + 30), float(h + 30) } * stuff::pixelSize;

	currencyNum = num;

	setupText(renderer);
}

void UfishNumWidget::setupText(SDL_Renderer* renderer) {
	TTF_Font* Sans = TTF_OpenFont("./fonts/pixelFont.ttf", 5 * stuff::pixelSize);
	SDL_Color White = { 255, 255, 255 };

	if (saveFish)
		surfaceText = TTF_RenderText_Solid(Sans, shortNumbers::convert2Short(saveFish->numOwned[fishQuality]).c_str(), White);
	else
		surfaceText = TTF_RenderText_Solid(Sans, shortNumbers::convert2Short(currencyNum).c_str(), White);

	text = SDL_CreateTextureFromSurface(renderer, surfaceText);

	SDL_QueryTexture(text, NULL, NULL, &textW, &textH);

	TTF_CloseFont(Sans);
}

void UfishNumWidget::draw(SDL_Renderer* renderer, float x, float y) {
	// SDL_Rect Message_rect = { int(18 * stuff::pixelSize), int(h * stuff::pixelSize / 2 + y - textH / 2), textW, textH };
	SDL_Rect Message_rect = { int(18 * stuff::pixelSize) + x, int(h * stuff::pixelSize / 2 + y - textH / 2), textW, textH };
	SDL_RenderCopy(renderer, text, NULL, &Message_rect);
	
	SDL_Rect fishRect = SDL_Rect{ int(x), int(y), int(w * stuff::pixelSize), int(h * stuff::pixelSize) };
	SDL_RenderCopy(renderer, texture, NULL, &fishRect);

	if (fishQuality != 0) {
		star->loc = vector{ x, y } + vector{10, 10} * stuff::pixelSize;
		star->draw(renderer, false);
	}
}
