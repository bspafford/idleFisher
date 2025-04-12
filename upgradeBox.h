#pragma once

#include "widget.h"

#include <functional>

class text;
class Ubutton;
class verticalBox;
struct FworldStruct;
struct FsaveWorldStruct;
struct FfishingRodStruct;
struct FsaveFishingRodStruct;
struct FbaitStruct;
struct FsaveBaitStruct;
struct FpetStruct;
struct FsavePetStruct;
struct FupgradeStruct;
struct FsaveUpgradeStruct;
struct FvaultUnlocksStruct;
struct FsaveVaultUnlocksStruct;

class UupgradeBox : public widget {
public:
	UupgradeBox(widget* parent, FworldStruct* worldStruct, FsaveWorldStruct* saveWorldStruct);
	UupgradeBox(widget* parent, FfishingRodStruct* fishingRodStruct, FsaveFishingRodStruct* saveFishingRodStruct);
	UupgradeBox(widget* parent, FbaitStruct* baitStruct, FsaveBaitStruct* saveBaitStruct);
	UupgradeBox(widget* parent, FpetStruct* data, FsavePetStruct* saveData);
	UupgradeBox(widget* parent, FupgradeStruct* data, FsaveUpgradeStruct* saveData);
	UupgradeBox(widget* parent, FvaultUnlocksStruct* data, FsaveVaultUnlocksStruct* saveData);
	~UupgradeBox();
	void setup();
	void update();

	void draw(SDL_Renderer* renderer) override;

	vector getSize() override;

private:
	void buyUpgrade();

	widget* widgetParent;
	widget* wrapBox;

	// structs
	FworldStruct* worldStruct;
	FsaveWorldStruct* saveWorldStruct;
	FfishingRodStruct* fishingRodStruct;
	FsaveFishingRodStruct* saveFishingRodStruct;
	FbaitStruct* baitStruct;
	FsaveBaitStruct* saveBaitStruct;
	FpetStruct* petStruct;
	FsavePetStruct* savePetStruct;
	FupgradeStruct* upgradeStruct;
	FsaveUpgradeStruct* saveUpgradeStruct;
	FvaultUnlocksStruct* vaultUnlocksStruct;
	FsaveVaultUnlocksStruct* saveVaultUnlocksStruct;

	Fimg* background;

	Fimg* thumbnailBackground;
	Fimg* thumbnail;
	text* name;

	std::string nameString;
	std::string descriptionString;
	std::string buffString;
	std::string debuffString;

	text* upgradeText;
	text* buttonPriceText;
	Fimg* currencyImg;
	int* currencyId;
	int* upgradeNum;
	int upgradeMax = 1;
	bool* unlocked;
	double* price;

public:
	Ubutton* buyButton;
protected:
	void setupLocs() override;
private:
	void setLocAndSize(vector loc, vector size) override;


	bool mouseOver();

	void openWorld();
	void spawnPet();
	void equipFishingRod();
	void equipBait();

	std::function<void()> callback;
};