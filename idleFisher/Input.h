#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

#include "math.h"

static struct MouseButton {
	static inline int left = GLFW_MOUSE_BUTTON_LEFT;
	static inline int right = GLFW_MOUSE_BUTTON_RIGHT;
	static inline int middle = GLFW_MOUSE_BUTTON_MIDDLE;
	static inline int backwards = GLFW_MOUSE_BUTTON_4;
	static inline int forwards = GLFW_MOUSE_BUTTON_5;
};

class Input {
public:
	Input();

	// callback functions
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

	// sets all Up and Down buttons to false, and calles glfwPollEvents()
	static void pollEvents();
	// calls the callback functions that were held to the end of the frame
	// example: leftClickCallback
	static void fireHeldInputs();

	// returns true if user pressess mouse button down that frame
	// 0 = left, 1 = right, 2 = middle, 3 = back side button, 4 = front side button
	static bool getMouseButtonDown(int mouseButton);
	// returns true if user releases mouse button that frame
	// 0 = left, 1 = right, 2 = middle, 3 = back side button, 4 = front side button
	static bool getMouseButtonUp(int mouseButton);
	// returns true for every frame the mouse button is held down for
	// 0 = left, 1 = right, 2 = middle, 3 = back side button, 4 = front side button
	static bool getMouseButtonHeld(int mouseButton);
	// returns mouse scroll direction that frame
	// 1 = scroll up, -1 = scroll down
	static int getMouseScrollDir();
	static vector getMousePos();

	template <class T> static void setLeftClick(T* const object, void (T::* const callback) ()) {
		leftClickCallback = std::bind_front(callback, object);
	}

private:
	// if mouse button was pressed that frame
	static inline bool mouseDown[5];
	// if mouse button released that frame
	static inline bool mouseUp[5];
	// returns true for every frame the mouse button is held down for
	static inline bool mouseHeld[5];
	static inline int mouseScrollDir = 0;
	static inline vector mousePos;

	// front most items that was clicked that frame
	static inline std::function<void()> leftClickCallback;
	// prevents players from clicking through UIs
	static inline bool checkValidInteract();
};