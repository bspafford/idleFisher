#pragma once

#include "widget.h"

class Ubutton;
class text;
class UhoverBox;

class UequippedWidget : public widget {
public:
	UequippedWidget();
	~UequippedWidget();
	void draw(Shader* shaderProgram) override;
	vector getSize() override;
	void setLoc(vector loc) override;
	void update();
	bool mouseOverSlot(std::string &name, std::string &description);
private:
	bool mouseOver(Image* img);

	Image* charImg;

	// fishing equipped
	Image* fishingRodEquipSlot;
	Image* baitEquipSlot;
	// clothes equipped
	Image* headEquipSlot;
	Image* shirtEquipSlot;
	Image* legsEquipSlot;
	Image* shoesEquipSlot;

	// fishing thumbnails
	Image* fishingRodEquip;
	Image* baitEquip;
	// clothes thumbnails
	Image* headEquip;
	Image* shirtEquip;
	Image* legsEquip;
	Image* shoesEquip;
};
