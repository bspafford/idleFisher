#pragma once

#include "verticalBox.h"

class UscrollBox : public verticalBox {
public:
	UscrollBox();

	void draw(SDL_Renderer* renderer) override;

	bool mouseOver();
	void scrolling();
	void scrolling(int mouseWheelDir);

	vector getLoc() override;
	void setLocAndSize(vector loc, vector size) override;
	void setLoc(vector loc) override;

	bool mouseDownPrev = false;

	vector mouseStartPos;
	vector startLoc;
};