#include "heldFishWidget.h"

#include <iostream>

#include "main.h"
#include "stuff.h"
#include "saveData.h"
#include "upgrades.h"
#include "fishNumWidget.h"

UheldFishWidget::~UheldFishWidget() {
	for (int i = 0; i < fishNumList.size(); i++)
		delete fishNumList[i];
	for (int i = 0; i < currencyList.size(); i++)
		delete currencyList[i];
}

void UheldFishWidget::updateList(SDL_Renderer* renderer, std::vector<FsaveFishData> saveFishList) {
	if (saveFishList.size() == 0)
		saveFishList = SaveData::saveData.fishData;
	fishList = saveFishList;

	float yOffset = stuff::pixelSize;
	// setup fish
	fishList = removeUnneededFish();
	for (int i = 0; i < fishNumList.size(); i++)
		delete fishNumList[i];
	fishNumList.clear();
	for (int i = 0; i < fishList.size(); i++) {
		for (int j = 0; j < fishList[i].numOwned.size(); j++) {
			if (fishList[i].numOwned[j] == 0)
				continue;

			// setup fishNumWidget
			UfishNumWidget* widget = new UfishNumWidget();
			fishNumList.push_back(widget);

			widget->y = yOffset;
			widget->setup(renderer, &SaveData::data.fishData[fishList[i].id], &fishList[i], j);
			yOffset += (widget->h + 1) * stuff::pixelSize;
		}
	}

	// adjusted for the line
	yOffset += 5 * stuff::pixelSize;

	// setup currency
	getCurrency();
	for (int i = 0; i < currencyList.size(); i++)
		delete currencyList[i];
	currencyList.clear();
	for (int i = 0; i < currency.size(); i++) {
		UfishNumWidget* widget = new UfishNumWidget();
		currencyList.push_back(widget);

		FcurrencyStruct* currencyStruct = &SaveData::data.currencyData[currency[i].x];

		widget->y = yOffset;
		widget->setup(renderer, currencyStruct, currency[i].y);
		yOffset += (widget->h + 1)* stuff::pixelSize;
	}

	size.x = 35 * stuff::pixelSize;
	size.y = yOffset;
}

void UheldFishWidget::draw(SDL_Renderer* renderer) {
	float yOffset = loc.y;
	for (int i = 0; i < fishNumList.size(); i++) {
		fishNumList[i]->draw(renderer, loc.x + stuff::pixelSize, yOffset);
		yOffset += (fishNumList[i]->h + 1) * stuff::pixelSize;
	}

	// draws line between fish and currency
	if (fishNumList.size() > 0) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		UfishNumWidget* lastFish = fishNumList[(int)fishNumList.size() - 1];
		int y = lastFish->y + lastFish->h * stuff::pixelSize + stuff::pixelSize * 2;
		SDL_Rect rect = SDL_Rect{ int(loc.x + stuff::pixelSize), int(yOffset + stuff::pixelSize), int(20 * stuff::pixelSize), int(stuff::pixelSize)};
		SDL_RenderFillRect(renderer, &rect);

		// adjusted for the line
		yOffset += 5 * stuff::pixelSize;
	}

	for (int i = 0; i < currencyList.size(); i++) {
		currencyList[i]->draw(renderer, loc.x + stuff::pixelSize, yOffset);
		yOffset += (currencyList[i]->h + 1) * stuff::pixelSize;
	}

	size.y = yOffset;
}

std::vector<FsaveFishData> UheldFishWidget::removeUnneededFish() {
	std::vector<FsaveFishData> temp;
	for (int i = 0; i < fishList.size(); i++) {
		if (fishList[i].calcCaughtFish() > 0)
			temp.push_back(fishList[i]);
	}

	return temp;

	/*
	// get all fish that player has
	fishList.clear();
	for (int i = 0; i < SaveData::data.fishData.size(); i++) {
		FsaveFishData saveFish = SaveData::saveData.fishData[i];
		if (saveFish.numOwned > 0) {
			FsaveFishData fish = SaveData::saveData.fishData[i];
			fishList.push_back(fish);
		}
	}
	*/
}

void UheldFishWidget::getCurrency() {

	// x == id, y == num
	currency.clear();
	std::vector<vector> currencyList;

	for (int i = 0; i < fishList.size(); i++) {
		FsaveFishData saveFish = fishList[i];
		FfishData fish = SaveData::data.fishData[saveFish.id];

		for (int j = 0; j < fishList[i].numOwned.size(); j++) {
			int index = currencyInList(fish.currencyId, currencyList);
			if (index != -1) { // in list
				currencyList[index].y += float(upgrades::getFishSellPrice(fish, j) * saveFish.numOwned[j]);
			} else {// need to add it to list
				currencyList.push_back({ (float)fish.currencyId, float(upgrades::getFishSellPrice(fish, j) * saveFish.numOwned[j]) });
			}
		}
	}

	currency = currencyList;
}

int UheldFishWidget::currencyInList(int id, std::vector<vector> currencyList) {
	for (int i = 0; i < currencyList.size(); i++) {
		if (currencyList[i].x == id)
			return i;
	}

	return -1;
}