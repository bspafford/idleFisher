#include "heldFishWidget.h"

#include <iostream>

#include "main.h"
#include "stuff.h"
#include "saveData.h"
#include "upgrades.h"
#include "fishNumWidget.h"
#include "Rectangle.h"

UheldFishWidget::UheldFishWidget(widget* parent) : widget(parent) {
	line = std::make_unique<URectangle>(vector{ 0, 0 }, vector{ 20 * stuff::pixelSize, stuff::pixelSize }, false);
}

UheldFishWidget::~UheldFishWidget() {

}

void UheldFishWidget::updateList(std::vector<FsaveFishData> saveFishList) {
	if (saveFishList.size() == 0)
		saveFishList = SaveData::saveData.fishData;
	fishList = saveFishList;

	float yOffset = stuff::pixelSize;
	// setup fish
	fishList = removeUnneededFish();
	fishNumList.clear();

	for (int i = 0; i < fishList.size(); i++) {
		for (int j = 0; j < fishList[i].numOwned.size(); j++) {
			if (fishList[i].numOwned[j] == 0)
				continue;

			// setup fishNumWidget
			std::unique_ptr<UfishNumWidget> widget = std::make_unique<UfishNumWidget>(this);

			widget->setLoc({ widget->getLoc().x, yOffset });
			widget->setup(&SaveData::data.fishData[fishList[i].id], &fishList[i], j);
			yOffset += widget->getSize().y + 1 * stuff::pixelSize;

			fishNumList.push_back(std::move(widget));
		}
	}

	// adjusted for the line
	yOffset += 5 * stuff::pixelSize;

	// setup currency
	getCurrency();
	currencyList.clear();
	for (int i = 0; i < currency.size(); i++) {
		std::unique_ptr<UfishNumWidget> widget = std::make_unique<UfishNumWidget>(this);

		FcurrencyStruct* currencyStruct = &SaveData::data.currencyData[currency[i].x];

		widget->setLoc({ widget->getLoc().x, yOffset });
		widget->setup(currencyStruct, currency[i].y);
		yOffset += widget->getSize().y + 1 * stuff::pixelSize;

		currencyList.push_back(std::move(widget));
	}

	size.x = 35 * stuff::pixelSize;
	size.y = yOffset;
}

void UheldFishWidget::draw(Shader* shaderProgram) {
	vector offset = vector{ 5, 5 } * stuff::pixelSize;

	float yOffset = loc.y;
	for (int i = 0; i < fishNumList.size(); i++) {
		fishNumList[i]->setLoc(vector{ loc.x, yOffset } + offset);
		fishNumList[i]->draw(shaderProgram);
		yOffset += fishNumList[i]->getSize().y + 1 * stuff::pixelSize;
	}

	// draws line between fish and currency
	if (fishNumList.size() > 0) {
		line->setLoc(vector{ loc.x, yOffset } + offset);
		line->draw(shaderProgram);

		// adjusted for the line
		yOffset += 5 * stuff::pixelSize;
	}

	for (int i = 0; i < currencyList.size(); i++) {
		currencyList[i]->setLoc(vector{ loc.x, yOffset } + offset);
		currencyList[i]->draw(shaderProgram);
		yOffset += currencyList[i]->getSize().y + 1 * stuff::pixelSize;
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
			} else { // need to add it to list
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