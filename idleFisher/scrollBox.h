#pragma once

#include "verticalBox.h"

class UscrollBox : public verticalBox {
public:
	UscrollBox(widget* parent);

	void draw(Shader* shaderProgram) override;

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