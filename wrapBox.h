#pragma once

#include "widget.h"

class UwrapBox : public widget {
public:
	UwrapBox(vector loc, vector size);

	void draw(SDL_Renderer* renderer);
	void addChild(widget* child);
	widget* getChildAt(int index);
	std::vector<widget*> getAllChildren();

	vector getOverflowSize();
private:
	std::vector<widget*> childrenList;

	float overflowSizeY = 0;
};