#include "button.h"
#include "main.h"
#include "sounds.h"
#include "animation.h"
#include "widget.h"

// only give NON NULL values to overriding widgets ONLY
Ubutton::Ubutton(widget* widget, std::string spriteSheetPath, int cellWidth, int cellHeight, int numberOfFrames, vector loc, bool useCharLoc, bool useSurface) {
	// buttons will have a sprite sheet with click anim on top, and hover right below it, and prolly also a disabled one too
	// needs:
	// sprite sheet path, cell size, number of frames

	widgetClass = widget;

	//buttonImg = new Fimg(path, loc);
	std::unordered_map<std::string, animDataStruct> animData;
	animData.insert({ "click", { { 0, 0 }, { float(numberOfFrames - 1), 0 }, .1, false } });
	animData.insert({ "hover", { { 0, 1 }, { float(numberOfFrames - 1), 1 }, .1, false } });
	animData.insert({ "disabled", { { 0, 2 }, { 1, 2 }, .1, false } });
	buttonAnim = new animation(spriteSheetPath, cellWidth, cellHeight, animData, useCharLoc, loc);
	buttonAnim->setAnimation("click");

	hasHover = buttonAnim->cellHeightNum >= 2;
	hasDisabled = buttonAnim->cellHeightNum >= 3;

	this->loc = loc;

	this->useSurface = useSurface;
	if (useSurface)
		buttonAnim->spriteSheet->loadSurface("./images/" + spriteSheetPath);

	this->useCharLoc = useCharLoc;
}

Ubutton::~Ubutton() {
	delete buttonAnim;
	delete surface;
	buttonAnim = nullptr;
	surface = nullptr;
}

void Ubutton::addCallback(void (*callback) ()) {
	callback_ = callback;
}

void Ubutton::draw(SDL_Renderer* renderer) {
	onHover(renderer);
}

void Ubutton::onHover(SDL_Renderer* renderer) {
	if (buttonAnim)
		buttonAnim->draw(Main::renderer);

	if (isMouseOver()) {
		// when going from nonHover to Hover
		if (!bIsHovered) {
			Main::hoverObject(widgetClass);
			if (isEnabled) {
				if (hasHover && buttonAnim)
					buttonAnim->setAnimation("hover");
			}
		}

		bIsHovered = true;
		if (Main::bLeftClick && (!Main::currWidget || widgetClass == Main::currWidget))
			onClick();
	} else {
		// when going from Hover to nonHover
		if (bIsHovered) {
			Main::leaveHoverObject(widgetClass);
			if (buttonAnim)
				//buttonImg->setImage(path, buttonImg->loc);
				buttonAnim->setAnimation("click");
		}
		bIsHovered = false;
	}
}

bool Ubutton::isMouseOver() {
	if (!buttonAnim)
		return false;

	vector mousePos = Main::mousePos;
	bool parentStuff = false;
	if (parent) {
		vector parentLoc = parent->ogLoc;
		vector parentSize = parent->getSize();
		parentStuff = mousePos.x >= loc.x + parentLoc.x && mousePos.x <= loc.x + buttonAnim->cellWidth * stuff::pixelSize + parentLoc.x && mousePos.y >= loc.y + parentLoc.y && mousePos.y <= loc.y + buttonAnim->cellHeight * stuff::pixelSize + parentLoc.y;
	}

	if (useSurface) {
		vector min = math::worldToScreen(buttonAnim->getLoc());
		vector max = min + (vector{ float(buttonAnim->cellWidth), float(buttonAnim->cellHeight) } *stuff::pixelSize);

		if (parentStuff || (!parent && min.x <= Main::mousePos.x && Main::mousePos.x <= max.x && min.y <= Main::mousePos.y && Main::mousePos.y <= max.y)) {
			vector screenPos = min;
			vector pos = { Main::mousePos.x - screenPos.x, Main::mousePos.y - screenPos.y };
			SDL_Color pixelColor = math::GetPixelColor(buttonAnim->spriteSheet->surface, (int)pos.x, (int)pos.y);

			if ((int)pixelColor.a != 0)
				return true;
		}
	} else {
		vector screenLoc = loc;
		if (useCharLoc)
			screenLoc = math::worldToScreen(loc);
		bool inX = Main::mousePos.x >= screenLoc.x && Main::mousePos.x <= screenLoc.x + buttonAnim->cellWidth * stuff::pixelSize;
		bool inY = Main::mousePos.y >= screenLoc.y && Main::mousePos.y <= screenLoc.y + buttonAnim->cellHeight * stuff::pixelSize;
		if (parentStuff || (!parent && inX && inY))
			return true;
	}

	return false;
}

void Ubutton::setLoc(vector loc) {
	__super::setLoc(loc);
	
	buttonAnim->setLoc(loc);
}

void Ubutton::onClick() {
	if (!isEnabled)
		return;

	if (hasHover && buttonAnim) {
	//if (clickAnim && clickImgs.size() > 0) {
		buttonAnim->setAnimation("hover");
		buttonAnim->start();
	}

	Mix_PlayChannel(-1, sounds::buttonClick, 0);

	if (callback_)
		Main::addLeftClick(callback_);
}

/*
void Ubutton::setImg(std::string path, std::string hoverPath) {
	//buttonImg->setImage(path, buttonImg->loc);
	//this->path = path;

	if (hoverPath != "") {
		//this->hoverPath = hoverPath;
	}
}
*/

void Ubutton::enable(bool enabled) {
	isEnabled = enabled;

	// sets it to normal animation if the button is hovered
	if (hasHover && !isEnabled && bIsHovered) {
		buttonAnim->setAnimation("hover");
	}
}

Fimg* Ubutton::getButtonImg() {
	if (buttonAnim)
		return buttonAnim->spriteSheet;
	return NULL;
}

void Ubutton::setParent(widget* parent) {
	this->parent = parent;
}

vector Ubutton::getSize() {
	return vector{ float(buttonAnim->cellWidth), float(buttonAnim->cellHeight) } * stuff::pixelSize;
}

vector Ubutton::getLoc() {
	return loc;
}