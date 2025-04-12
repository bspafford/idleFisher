#pragma once

#include "widget.h"

class Ubutton;
class text;
class UrebirthUnlock;
class UhoverBox;

class UrebirthWidget : public widget {
public:
	UrebirthWidget();
	~UrebirthWidget();

	void draw(SDL_Renderer* renderer) override;
	void update();

private:
	bool isMouseOverUnlock();
	void scrolling();
	void setupLocs() override;
	void addedToViewport() override;
	void drawLine(SDL_Renderer* renderer, vector loc1, vector loc2);

	bool mouseDownPrev = false;
	vector scrollLoc;
	vector startLoc;
	vector mouseStartPos;

	std::vector<UrebirthUnlock*> rebirthButtonList;

	Ubutton* xButton;
	Ubutton* rebirthButton;
	text* rebirthText;
	text* rebirthNum;

	bool wasOverButton = false;
	bool justOpened = true;

	UhoverBox* hoverBox;
};