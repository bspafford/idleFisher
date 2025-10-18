#include "widget.h"
#include <iostream>
#include "main.h"
#include "character.h"
#include "Image.h"
#include "shaderClass.h"

// temp
#include "worlds.h"
#include "scuba.h"

widget::widget(widget* parent) {
	instances.insert(this);
	this->parent = parent;
}

widget::~widget() {
	instances.erase(this);
}

void widget::addToViewport(bool override) {
	if (override) {
		if (Main::currWidget && Main::currWidget != this)
			Main::currWidget->removeFromViewport();
		
		addedToViewport();

		Main::currWidget = this;
		
		Main::character->setCanMove(false);
	}

	visible = true;
}

void widget::removeFromViewport() {
	Main::setMouseImg("cursor");

	if (Main::currWidget == this) {
		// keep widget in memory until change the world
		// then once change the world load all the widgets of that world back into memory during the loading
		//delete Main::currWidget; // removes widget from memory
		Main::currWidget = nullptr;
	}

	Main::character->setCanMove(true);

	visible = false;
}

void widget::draw(Shader* shaderProgram) {

}

bool widget::isVisible() {
	return visible;
}

void widget::setVisibility(bool visible) {
	this->visible = visible;
}

bool widget::mouseOver() {
	vector mousePos = Main::mousePos;
	vector loc = getLoc();
	vector size = getSize();
	if (mousePos.x >= loc.x && mousePos.x <= loc.x + size.x && mousePos.y >= loc.y && mousePos.y <= loc.y + size.y)
		return true;
	return false;
}

void widget::resizeScreen() {
	for (widget* wig : instances)
		wig->setupLocs();
}

void widget::setupLocs() {

}

vector widget::getLoc() {
	if (parent)
		return loc;
	return loc;
}

vector widget::getSize() {
	return size;
}

void widget::setLoc(vector loc) {
	this->loc = loc;
}

void widget::setOgLoc(vector loc) {
	ogLoc = loc;
}

void widget::setSize(vector size) {
	this->size = size;

}
void widget::setLocAndSize(vector loc, vector size) {
	setLoc(loc);
	setSize(size);
}

void widget::addedToViewport() {

}

widget* widget::getParent() {
	return parent;
}

void widget::setParent(widget* parent) {
	this->parent = parent;
}