#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "upgradeBox.h"
#include "button.h"

class text;
class verticalBox;

class UfishermanWidget : public widget {
public:
	UfishermanWidget(class npc* parent);
	~UfishermanWidget();

	void draw(Shader* shaderProgram) override;
	void setup();
	void setupLocs() override;
	void showUpgradePage(int index);

	void setNameDescription(std::string name, std::string buffString, std::string debuffString);

	void updateStats();
	void upgradePower();
	void upgradeSpeed();
	void upgradeCatchChance();

	class npc* parent;

	// fishing rod page
	Image* fishingRodThumbnail;
	text* powerText;
	Ubutton* powerUpgradeButton;
	text* speedText;
	Ubutton* speedUpgradeButton;
	text* catchChanceText;
	Ubutton* catchChanceUpgradeButton;
	text* powerButtonPrice;
	text* speedButtonPrice;
	text* catchChanceButtonPrice;

	text* powerLevelText;
	text* speedLevelText;
	text* catchChanceLevelText;

	// fishing rod stats
	text* powerStatsText;
	text* powerStatsTextNum;
	text* speedStatsText;
	text* speedStatsTextNum;
	text* catchChanceStatsText;
	text* catchChanceStatsTextNum;



	// upgrades
	UscrollBox* baitHolderList;
	std::vector<Ubutton*> buttonList;
	Image* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* buffText;
	text* debuffText;
	Image* infoBackground;

	// npc img
	Image* npcImg;
	Image* npcBackground;

	Image* selectedIcon;

	Ubutton* closeButton;

private:
	int selectedPageIndex = 0;

	void setPage1() {
		showUpgradePage(0);
	}
	void setPage2() {
		showUpgradePage(1);
	}
};
