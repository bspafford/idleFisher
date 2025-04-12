#pragma once

#include "SDL.h"

#include <vector>

#include "math.h"
#include "widget.h"
#include "saveData.h"

class UfishNumWidget;

class UheldFishWidget : public widget {
public:
	~UheldFishWidget();

	// if fishList is empty it will use SaveData::data.fishData
	void updateList(SDL_Renderer* renderer, std::vector<FsaveFishData> saveFishList = std::vector<FsaveFishData>(0));

	void draw(SDL_Renderer* renderer);
	// removes all fish that there are 0 of
	std::vector<FsaveFishData> removeUnneededFish();
	void getCurrency();
	int currencyInList(int id, std::vector<vector> currencyList);

	std::vector<FsaveFishData> fishList;
	std::vector<vector> currency;

	std::vector<UfishNumWidget*> fishNumList;
	std::vector<UfishNumWidget*> currencyList;

	double numOwned = 0;
};