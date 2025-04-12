#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "upgradeBox.h"
#include "button.h"

class text;
class verticalBox;
class UprogressBar;
struct FmechanicStruct;
struct FsaveMechanicStruct;

class UmechanicWidget : public widget {
public:
	UmechanicWidget(class npc* parent);
	~UmechanicWidget();

	void draw(SDL_Renderer* renderer) override;
	void setupLocs() override;
	void setup();
	void setNameDescription(std::string name, std::string description);
	void buyFishTransporter();
	void upgradeFishTransporter();
	void update();

	double calcUpgradeCost();

	class npc* parent;

	FmechanicStruct* mechanicStruct;
	FsaveMechanicStruct* saveMechanicStruct;

	// upgrades
	Fimg* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* description;
	Fimg* infoBackground;

	// npc img
	Fimg* npcImg;
	Fimg* npcBackground;

	Ubutton* closeButton;

	// fish transporter
	Ubutton* buyFishTransporterButton;
	text* buyFishTransporterText;
	text* buyFishTransporterPriceText;

	// bought screen
	text* fishTransporterName;
	Fimg* fishTransporterImg;
	text* level;
	UprogressBar* levelProgress;
	text* maxHoldText;
	text* maxHoldValue;
	text* speedText;
	text* speedValue;
	text* collectSpeedText;
	text* collectSpeedValue;
	Ubutton* buyButton;
	Ubutton* multi1x;
	Ubutton* multi10x;
	Ubutton* multiMax;

	text* upgradePriceText;
	Fimg* currencyIcon;
};
