#include "sailorWidget.h"
#include "main.h"
#include "saveData.h"
#include "shortNumbers.h"
#include "currencyWidget.h"
#include "button.h"
#include "text.h"
#include "map.h"
#include "animation.h"
#include "timer.h"

UsailorWidget::UsailorWidget() {
	mapBackground = new Fimg("./images/widget/map.png");
	setupLocs(); // setup so map can get loc of map background
	map = new Umap(this);

	xButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	xButton->addCallback(this, &UsailorWidget::closeWidget);

	mapClosed = new Fimg("./images/widget/mapClosed.png");

	std::unordered_map < std::string, animDataStruct> anim;
	anim.insert({ "open", { {0, 0}, {5, 0}, .1, false} });
	mapOpenAnim = new animation("widget/mapSpriteSheet.png", 402, 230, anim, false, { 0, 0 });
	mapOpenAnim->setAnimation("open");

	mapTimer = new timer();
	mapTimer->addUpdateCallback(this, &UsailorWidget::moveAnim);
	mapTimer->addCallback(this, &UsailorWidget::finishmapAnim);

	setupLocs();
}

UsailorWidget::~UsailorWidget() {
	delete mapBackground;
	delete xButton;
	delete map;
}

void UsailorWidget::draw(SDL_Renderer* renderer) {
	if (mapClosed && !mapTimer->finished())
		mapClosed->draw(renderer, false);
	if (mapOpenAnim && !mapOpenAnim->finished() && mapTimer->finished())
		mapOpenAnim->draw(renderer);

	if (!mapOpenAnim || !mapOpenAnim->finished() || !mapTimer->finished())
		return;

	__super::draw(renderer);

	mapBackground->draw(renderer, false);

	// draw only inside specified area
	vector mapSize = mapBackground->getSize() - vector{ 70, 70 } * stuff::pixelSize;
	SDL_Texture* texture = SDL_CreateTexture(Main::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mapSize.x, mapSize.y);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	map->draw(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rect = SDL_Rect{ int(mapBackground->loc.x + 35 * stuff::pixelSize), int(mapBackground->loc.y + 35 * stuff::pixelSize), int(mapSize.x), int(mapSize.y) };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);

	if (xButton) {
		xButton->draw(renderer);
	}
}

void UsailorWidget::setupLocs() {
	if (mapBackground)
		mapBackground->loc = vector{ Main::screenWidth / 2.f, Main::screenHeight / 2.f } - mapBackground->getSize() / 2.f;
	if (xButton)
		xButton->setLoc(mapBackground->loc + vector{ mapBackground->getSize().x - 13.5f * stuff::pixelSize, 5 * stuff::pixelSize});

	if (mapOpenAnim && mapBackground)
		mapOpenAnim->setLoc(mapBackground->loc);
}

void UsailorWidget::addedToViewport() {
	mapClosed->loc = Main::getScreenSize();
	mapTimer->start(.5);
}

void UsailorWidget::moveAnim() {
	float percent = mapTimer->getTime() / mapTimer->getMaxTime();

	std::vector<vector> locs = { {0, 1000}, { 0, 500 }, {0, 100}, {0, 30}, {0, 10}, {0, 0}, {0, 0} };
	mapClosed->loc = mapBackground->loc + locs[floor(percent * 6)];
}

void UsailorWidget::finishmapAnim() {
	mapOpenAnim->start();
}

void UsailorWidget::closeWidget() {
	removeFromViewport();
}