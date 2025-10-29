#include "cursor.h"
#include "main.h"
#include "Input.h"
#include "Hoverable.h"
#include "textureManager.h"

#include "debugger.h"

void Cursor::calcMouseImg() {
	IHoverable* hoveredItem = IHoverable::getHoveredItem();
	bool canHover = IHoverable::checkValidInteract();
	if (hoveredItem && canHover) {
		std::string hoveredIcon = hoveredItem->getMouseHoverIcon();
		if (Input::getMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {
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

	if (SaveData::settingsData.cursor) {
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
	} else {
		GLFWcursor* handCursor = glfwCreateStandardCursor(cursorLookup[cursorName]);
		glfwSetCursor(Main::window, handCursor);
	}
}

bool Cursor::getMouseOverWater() {
	return mouseOverWater;
}

void Cursor::setMouseOverWater(bool overWater) {
	mouseOverWater = overWater;
}

void Cursor::toggleCursor() {
	glfwSetCursor(Main::window, NULL);
}