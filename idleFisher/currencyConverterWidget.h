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

	void draw(Shader* shaderProgram) override;
	void setup();
	void setupLocs() override;

	void setNameDescription(std::string name, std::string description);

	void addedToViewport() override;

	class npc* parent;

	// upgrades
	std::unique_ptr<UscrollBox> upgradeHolder;
	std::unique_ptr<Image> upgradeBackground;
	std::vector<std::unique_ptr<UcurrencyConverterBox>> currencyConverterBoxList;

	// stats
	std::unique_ptr<verticalBox> nameHolder;
	std::unique_ptr<text> name;
	std::unique_ptr<text> description;
	std::unique_ptr<Image> infoBackground;

	// npc img
	std::unique_ptr<Image> npcImg;
	std::unique_ptr<Image> npcBackground;

	std::unique_ptr<Ubutton> closeButton;
};
