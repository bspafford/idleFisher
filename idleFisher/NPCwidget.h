#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "upgradeBox.h"
#include "button.h"

class text;
class verticalBox;
class Shader;
class Image;

class NPCwidget : public widget {
public:
	NPCwidget(class npc* parent, std::string npcName);
	~NPCwidget();

	void draw(Shader* shaderProgram) override;
	void setupLocs() override;

	void setNameDescription(std::string name, std::string description);

	class npc* parent;

	// upgrades
	UscrollBox* upgradeHolder;
	Image* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* description;
	Image* infoBackground;

	// npc img
	Image* npcImg;
	Image* npcBackground;

	Ubutton* closeButton;

	template <typename T1, typename T2>
	void setup(std::vector<T1>& data, std::vector<T2>& saveData) {
		for (int i = 0; i < data.size(); i++) {
			T1* currData = &data[i];
			T2* currSaveData = &saveData[currData->id];
			UupgradeBox* upgradeBox = new UupgradeBox(this, currData, currSaveData);
			if (upgradeBox->buyButton)
				upgradeBox->buyButton->setParent(upgradeHolder);
			upgradeHolder->addChild(upgradeBox, upgradeBox->getSize().y);
		}

		setupLocs();
	}
};
