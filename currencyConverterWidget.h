#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "currencyConverterBox.h"
#include "button.h"

class text;
class verticalBox;

class UcurrencyConverterWidget : public widget {
public:
	UcurrencyConverterWidget(class npc* parent);
	~UcurrencyConverterWidget();

	void draw(SDL_Renderer* renderer) override;
	void setup();
	void setupLocs() override;

	void setNameDescription(std::string name, std::string description);

	void addedToViewport() override;

	class npc* parent;

	// upgrades
	UscrollBox* upgradeHolder;
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
};
