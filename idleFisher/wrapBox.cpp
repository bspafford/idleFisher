#include "wrapBox.h"

UwrapBox::UwrapBox(widget* parent, vector loc, vector size) : widget(parent) {
	this->loc = loc;
	ogLoc = loc;
	this->size = size;
}

void UwrapBox::draw(Shader* shaderProgram) {
	vector offset = loc + vector{ 0, stuff::pixelSize };

	for (widget* child : childrenList) {
		// need to update position depending on index in horizontal box
		vector childSize = child->getSize();
		if (offset.x + childSize.x > size.x + loc.x - stuff::pixelSize) { // if loc + size is out of box, then change y and set x to loc.x
			offset.x = loc.x;
			offset.y += childSize.y;
		}

		child->setLoc(offset);
		child->draw(shaderProgram);
		offset.x += childSize.x;
	}
}

void UwrapBox::addChild(widget* child) {
	child->setParent(this);
	childrenList.push_back(child);
}

widget* UwrapBox::getChildAt(int index) {
	return childrenList[index];
}

std::vector<widget*> UwrapBox::getAllChildren() {
	return childrenList;
}

vector UwrapBox::getOverflowSize() {
	vector offset = vector{ stuff::pixelSize, stuff::pixelSize };

	for (widget* child : childrenList) {
		// need to update position depending on index in horizontal box
		vector childSize = child->getSize();
		if (offset.x + childSize.x > size.x + loc.x - stuff::pixelSize) { // if loc + size is out of box, then change y and set x to loc.x
			offset.x = loc.x;
			offset.y += childSize.y;
		}

		offset.x += childSize.x;
	}

	if (childrenList.size() > 0)
		overflowSizeY = offset.y + childrenList[int(childrenList.size()) - 1]->getSize().y;
	else
		overflowSizeY = 0;

	return { size.x, overflowSizeY };
}