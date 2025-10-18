#include "scrollBox.h"

#include "main.h"

UscrollBox::UscrollBox(widget* parent) : verticalBox(parent) {
}

void UscrollBox::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	vector scrollLoc = ogLoc;
	glEnable(GL_SCISSOR_TEST);
	//glScissor(scrollLoc.x, stuff::screenSize.y - pixelSize.y + scrollLoc.y, pixelSize.x, pixelSize.y);
	glScissor(scrollLoc.x, stuff::screenSize.y - size.y - scrollLoc.y, size.x, size.y);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	__super::draw(shaderProgram);

	glDisable(GL_SCISSOR_TEST);

	if (mouseOver() && Main::bRightMouseButtonDown) {
		if (!mouseDownPrev) {
			startLoc = loc;
			mouseStartPos = Main::mousePos;
		}
		scrolling();
	} else if (mouseOver() && Main::mouseWheelDir != 0) {
		scrolling(Main::mouseWheelDir);
	}

	mouseDownPrev = Main::bRightMouseButtonDown;
}

// mouse right click scrolling
void UscrollBox::scrolling() {
	if (overflowSizeY < size.y) {
		loc.y = -overflowSizeY + size.y;
		return;
	}

	vector diff = mouseStartPos - startLoc;
	loc.y = math::clamp(Main::mousePos.y - diff.y, -overflowSizeY + size.y, 0);

	std::cout << "loc: " << loc << ", mouse pos diff: " << Main::mousePos.y - diff.y << ", " << -overflowSizeY << " + " << size.y << std::endl;

	// need to set location of all the children
	float yOffset = 0;
	for (vertChildComp comp : childList) {
		if (comp.child)
			comp.child->setLocAndSize({ ogLoc.x, ogLoc.y + loc.y + yOffset}, comp.child->size);
		yOffset += comp.widgetHeight;
	}
}

void UscrollBox::scrolling(int mouseWheelDir) {
	if (overflowSizeY < size.y) {
		loc.y = -overflowSizeY + size.y;
		return;
	}

	vector diff = vector{ 0, mouseWheelDir * 10.f * stuff::pixelSize } + loc;
	loc.y = math::clamp(diff.y, -overflowSizeY + size.y, 0);

	float yOffset = 0;
	for (vertChildComp comp : childList) {
		if (comp.child)
			comp.child->setLocAndSize({ ogLoc.x, ogLoc.y + loc.y + yOffset }, comp.child->size);
		yOffset += comp.widgetHeight;
	}
}

bool UscrollBox::mouseOver() {
	vector mousePos = Main::mousePos;
	if (mousePos.x >= ogLoc.x && mousePos.x <= ogLoc.x + size.x && mousePos.y >= ogLoc.y && mousePos.y <= ogLoc.y + size.y)
		return true;
	return false;
}

vector UscrollBox::getLoc() {
	return ogLoc;
}

void UscrollBox::setLoc(vector loc) {
	ogLoc = loc;
}

void UscrollBox::setLocAndSize(vector loc, vector size) {
	setLoc(loc);
	setSize(size);

	float yOffset = 0;
	for (vertChildComp comp : childList) {
		// need to update position depending on index in horizontal box
		if (comp.child)
			comp.child->setLocAndSize({ ogLoc.x, ogLoc.y + yOffset }, { size.x, comp.widgetHeight });
		yOffset += comp.widgetHeight;
	}

	overflowSizeY = yOffset;
}