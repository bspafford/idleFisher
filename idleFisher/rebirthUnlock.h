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
	void draw(Shader* shaderProgram) override;
	bool isMouseOver();
	void setLoc(vector loc) override;
	vector getSize() override;

private:
	void unlock();
	void onClick();
	bool prerequisitesMet();

	Image* background;
	Ubutton* button;
	text* upgradeCost;

	FrebirthStruct* rebirthInfo;
	FsaveRebirthStruct* saveRebirthInfo;
};