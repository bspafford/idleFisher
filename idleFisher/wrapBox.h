#pragma once

#include "widget.h"

class UwrapBox : public widget {
public:
	UwrapBox(widget* parent, vector loc, vector size);

	void draw(Shader* shaderProgram);
	void addChild(widget* child);
	widget* getChildAt(int index);
	std::vector<widget*> getAllChildren();

	vector getOverflowSize();
private:
	std::vector<widget*> childrenList;

	float overflowSizeY = 0;
};