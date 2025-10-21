#include "cursor.h"
#include "main.h"
#include "Input.h"
#include "Hoverable.h"
#include "textureManager.h"

void Cursor::calcMouseImg() {
	bool canHover = IHoverable::checkValidInteract();
	IHoverable* hoveredItem = IHoverable::getHoveredItem();
	if (hoveredItem && canHover) {
		if (Input::getMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {
			std::string hoveredIcon = hoveredItem->getMouseHoverIcon();
			if (hoveredIcon == "cursor1")
				setMouseImg("cursor4");
			else if (hoveredIcon == "cursor2")
				setMouseImg("cursor3");
		} else
			setMouseImg(hoveredItem->getMouseHoverIcon());
	} else if (!hoveredItem && mouseOverWater)
		setMouseImg("hook");
	else if (!hoveredItem || !canHover)
		setMouseImg("cursor");

	// resets hoveredItem
	IHoverable::setHoveredItem(nullptr);
}

void Cursor::setMouseImg(std::string cursorName) {
	// return if same cursor
	if (currCursor == cursorName)
		return;

	currCursor = cursorName;

	int width, height, channels;

	// should be getting texture from texture manager not loading them ever time i switch
	textureStruct* mouseImg = textureManager::getTexture("./images/" + currCursor + ".png");
	GLFWimage cursorImg;
	cursorImg.width = mouseImg->w;
	cursorImg.height = mouseImg->h;
	cursorImg.pixels = mouseImg->texture;

	cursor = glfwCreateCursor(&cursorImg, 0, 0);
	if (cursor)
		glfwSetCursor(Main::window, cursor);
}

bool Cursor::getMouseOverWater() {
	return mouseOverWater;
}

void Cursor::setMouseOverWater(bool overWater) {
	mouseOverWater = overWater;
}