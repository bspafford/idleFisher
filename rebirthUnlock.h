#pragma once

#include "widget.h"

class text;
class Ubutton;
class FrebirthStruct;
class FsaveRebirthStruct;

class UrebirthUnlock : public widget {
public:
	UrebirthUnlock(widget* parent, int id);
	~UrebirthUnlock();
	void draw(SDL_Renderer* renderer) override;
	bool isMouseOver();
	void setLoc(vector loc) override;
	vector getSize() override;

private:
	void unlock();
	void onClick();
	bool prerequisitesMet();

	Fimg* background;
	Ubutton* button;
	text* upgradeCost;

	FrebirthStruct* rebirthInfo;
	FsaveRebirthStruct* saveRebirthInfo;
};