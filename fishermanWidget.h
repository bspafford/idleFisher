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

	void draw(SDL_Renderer* renderer) override;
	void setup();
	void setupLocs() override;
	void showUpgradePage(int index);

	void setNameDescription(std::string name, std::string buffString, std::string debuffString);

	class npc* parent;

	// upgrades
	std::vector<UscrollBox*> upgradeHolderList;
	std::vector<Ubutton*> buttonList;
	Fimg* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* buffText;
	text* debuffText;
	Fimg* infoBackground;

	// npc img
	Fimg* npcImg;
	Fimg* npcBackground;

	Fimg* selectedIcon;

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
