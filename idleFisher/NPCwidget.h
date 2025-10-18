#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "upgradeBox.h"
#include "button.h"

class npc;
class text;
class verticalBox;
class Shader;
class Image;

class NPCwidget : public widget {
public:
	NPCwidget(widget* parent, npc* NPCParent, std::string npcName);
	~NPCwidget();

	void draw(Shader* shaderProgram) override;
	void setupLocs() override;

	void setNameDescription(std::string name, std::string description);

	npc* NPCParent;

	// upgrades
	std::unique_ptr<UscrollBox> upgradeHolder;
	std::unique_ptr<Image> upgradeBackground;

	// stats
	std::unique_ptr<verticalBox> nameHolder;
	std::unique_ptr<text> name;
	std::unique_ptr<text> description;
	std::unique_ptr<Image> infoBackground;

	// npc img
	std::unique_ptr<Image> npcImg;
	std::unique_ptr<Image> npcBackground;

	std::unique_ptr<Ubutton> closeButton;

	std::vector<std::unique_ptr<UupgradeBox>> upgradeBoxList;

	template <typename T1, typename T2>
	void setup(std::vector<T1>& data, std::vector<T2>& saveData) {
		for (int i = 0; i < data.size(); i++) {
			T1* currData = &data[i];
			T2* currSaveData = &saveData[currData->id];
			std::unique_ptr<UupgradeBox> upgradeBox = std::make_unique<UupgradeBox>(upgradeHolder.get(), this, currData, currSaveData);
			if (upgradeBox->buyButton)
				upgradeBox->buyButton->setParent(upgradeHolder.get());
			upgradeHolder->addChild(upgradeBox.get(), upgradeBox->getSize().y);
			upgradeBoxList.push_back(std::move(upgradeBox));
		}

		setupLocs();
	}
};
