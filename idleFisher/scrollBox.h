#pragma once

#include "verticalBox.h"
#include "Hoverable.h"

class UscrollBox : public verticalBox, public IHoverable {
public:
	UscrollBox(widget* parent);

	void draw(Shader* shaderProgram) override;

	bool mouseOver();
	void scrolling();
	void scrolling(int mouseWheelDir);

	vector getLoc() override;
	void setLocAndSize(vector loc, vector size) override;
	void setLoc(vector loc) override;

	vector mouseStartPos;
	vector startLoc;
};