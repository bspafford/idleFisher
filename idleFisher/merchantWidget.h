#pragma once

#include "widget.h"

#include "scrollBox.h"
#include "upgradeBox.h"
#include "button.h"

class text;
class verticalBox;

class UmerchantWidget : public widget {
public:
	UmerchantWidget(class npc* parent);
	~UmerchantWidget();

	void draw(Shader* shaderProgram) override;
	void setup();
	void setupLocs() override;
	void showUpgradePage(int index);

	void setNameDescription(std::string name, std::string description);

	class npc* parent;

	// upgrades
	std::vector<UscrollBox*> upgradeHolderList;
	std::vector<Ubutton*> worldButtonList;
	Image* upgradeBackground;

	// stats
	verticalBox* nameHolder;
	text* name;
	text* description;
	Image* infoBackground;

	// npc img
	Image* npcImg;
	Image* npcBackground;

	Image* selectedWorldIcon;

	Ubutton* closeButton;

private:
	int selectedPageIndex = 0;

	void setPage1() {
		showUpgradePage(0);
	}
	void setPage2() {
		showUpgradePage(1);
	}
	void setPage3() {
		showUpgradePage(2);
	}
	void setPage4() {
		showUpgradePage(3);
	}
	void setPage5() {
		showUpgradePage(4);
	}
	void setPage6() {
		showUpgradePage(5);
	}
	void setPage7() {
		showUpgradePage(6);
	}
	void setPage8() {
		showUpgradePage(7);
	}
	void setPage9() {
		showUpgradePage(8);
	}
	void setPage10() {
		showUpgradePage(9);
	}
};
