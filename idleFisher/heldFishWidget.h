#pragma once

#include <vector>

#include "math.h"
#include "widget.h"
#include "saveData.h"
#include "fishNumWidget.h"

class Shader;
class URectangle;

class UheldFishWidget : public widget {
public:
	UheldFishWidget();
	~UheldFishWidget();

	// if fishList is empty it will use SaveData::data.fishData
	void updateList(std::vector<FsaveFishData> saveFishList = std::vector<FsaveFishData>(0));

	void draw(Shader* shaderProgram);
	// removes all fish that there are 0 of
	std::vector<FsaveFishData> removeUnneededFish();
	void getCurrency();
	int currencyInList(int id, std::vector<vector> currencyList);

	std::vector<FsaveFishData> fishList;
	std::vector<vector> currency;

	std::vector<std::unique_ptr<UfishNumWidget>> fishNumList;
	std::vector< std::unique_ptr<UfishNumWidget>> currencyList;

	double numOwned = 0;

	std::unique_ptr<URectangle> line;
};