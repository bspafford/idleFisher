#include "scrollBox.h"

#include "main.h"

UscrollBox::UscrollBox() {
}

void UscrollBox::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	SDL_Texture* texture = SDL_CreateTexture(Main::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	__super::draw(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_Rect rect = SDL_Rect{ int(ogLoc.x), int(ogLoc.y), int(size.x), int(size.y) };
	SDL_RenderCopy(renderer, texture, NULL, &rect);

	// removes texture from memory
	SDL_DestroyTexture(texture);
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

	// need to set location of all the children
	float yOffset = 0;
	for (vertChildComp comp : childList) {
		if (comp.child)
			comp.child->setLocAndSize({0, loc.y + yOffset}, comp.child->size);
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
			comp.child->setLocAndSize({ 0, loc.y + yOffset }, comp.child->size);
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
			comp.child->setLocAndSize({ 0, yOffset }, { size.x, comp.widgetHeight });
		yOffset += comp.widgetHeight;
	}

	overflowSizeY = yOffset;
}