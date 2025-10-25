#pragma once

#include "widget.h"

class IHoverable {
public:
	virtual ~IHoverable() {
		if (hoveredItem == this)
			hoveredItem = nullptr;
	}

	// set what the mouse hover icon should be
	void setMouseHoverIcon(std::string mouseName) {
		mouse = mouseName;
	}
	std::string getMouseHoverIcon() {
		return mouse;
	}

	static IHoverable* getHoveredItem() {
		return hoveredItem;
	}
	static void setHoveredItem(IHoverable* item) {
		hoveredItem = item;
	}
	// checks to see if hoveredItem is a widget
	// if it is a widget then it will see if it's part of the same tree that the currWidget is in
	// if it is part of the same tree then the funciton will return true, if not then it will return false
	// if hoveredItem is not a widget, then it will return true
	static bool checkValidInteract() {
		if (!hoveredItem)
			return false;
		// if object is widget, then check if its part of the override widget
		// if no overriding widget left click
		widget* _widget = dynamic_cast<widget*>(hoveredItem);
		if (!widget::getCurrWidget() || (_widget && _widget->getRootParent() == widget::getCurrWidget()))
			return true;
		return false;
	}

private:
	// which mouse should be active on hover
	std::string mouse = "cursor1";

	static inline IHoverable* hoveredItem;
};