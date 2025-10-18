#pragma once

#include "widget.h"

#include <vector>

struct horizChildComp {
	widget* child;
	float widgetWidth;
};

class horizontalBox : public widget {
public:
	horizontalBox(widget* parent);

	virtual void draw(Shader* shaderProgram) override;

	void addChild(widget* child, float widgetWidth);
	void removeChild(widget* child);
	void removeChild(int index);
	float getOverflowSize();
	void changeChildWidth(widget* child, float newWidth);

	void setLocAndSize(vector loc, vector size) override;
	void setOgLoc(vector ogLoc) override;

	std::vector<horizChildComp> childList;

	// is the size of the components inside the widget
	float overflowSizeX = 0;
};
