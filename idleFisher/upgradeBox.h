#pragma once

#include "widget.h"

#include <functional>

class Image;
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
	UupgradeBox(widget* parent, widget* NPCWidget, FworldStruct* worldStruct, FsaveWorldStruct* saveWorldStruct);
	UupgradeBox(widget* parent, widget* NPCWidget, FbaitStruct* baitStruct, FsaveBaitStruct* saveBaitStruct);
	UupgradeBox(widget* parent, widget* NPCWidget, FpetStruct* data, FsavePetStruct* saveData);
	UupgradeBox(widget* parent, widget* NPCWidget, FupgradeStruct* data, FsaveUpgradeStruct* saveData);
	UupgradeBox(widget* parent, widget* NPCWidget, FvaultUnlocksStruct* data, FsaveVaultUnlocksStruct* saveData);
	~UupgradeBox();
	void setup();
	void update();

	void draw(Shader* shaderProgram) override;

	vector getSize() override;

private:
	void buyUpgrade();

	widget* NPCWidget;

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

	std::unique_ptr<Image> background;

	std::unique_ptr<Image> thumbnailBackground;
	std::unique_ptr<Image> thumbnail;
	std::unique_ptr<text> name;

	std::string nameString;
	std::string descriptionString;
	std::string buffString;
	std::string debuffString;

	std::unique_ptr<text> upgradeText;
	std::unique_ptr<text> buttonPriceText;
	std::unique_ptr<Image> currencyImg;
	int* currencyId;
	int* upgradeNum;
	int upgradeMax = 1;
	bool* unlocked;
	double* price;
	double priceFallback;

public:
	std::unique_ptr<Ubutton> buyButton;
protected:
	void setupLocs() override;
private:
	void setLocAndSize(vector loc, vector size) override;


	bool mouseOver();

	void openWorld();
	void spawnPet();
	void equipBait();

	std::function<void()> callback;
};