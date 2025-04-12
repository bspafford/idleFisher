#pragma once

#include "widget.h"

class Ubutton;
class text;
class UhoverBox;

class UequippedWidget : public widget {
public:
	UequippedWidget();
	~UequippedWidget();
	void draw(SDL_Renderer* renderer) override;
	vector getSize() override;
	void setLoc(vector loc) override;
	void update();
	bool mouseOverSlot(std::string &name, std::string &description);
private:
	bool mouseOver(Fimg* img);

	Fimg* charImg;

	// fishing equipped
	Fimg* fishingRodEquipSlot;
	Fimg* baitEquipSlot;
	// clothes equipped
	Fimg* headEquipSlot;
	Fimg* shirtEquipSlot;
	Fimg* legsEquipSlot;
	Fimg* shoesEquipSlot;

	// fishing thumbnails
	Fimg* fishingRodEquip;
	Fimg* baitEquip;
	// clothes thumbnails
	Fimg* headEquip;
	Fimg* shirtEquip;
	Fimg* legsEquip;
	Fimg* shoesEquip;
};
