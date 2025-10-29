#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWcursor;

class Cursor {
public:
	static void calcMouseImg();

	static bool getMouseOverWater();
	static void setMouseOverWater(bool overWater);

	static void toggleCursor();

private:
	static void setMouseImg(std::string cursorName);

	static inline GLFWcursor* cursor;
	static inline std::string currCursor;

	// if mouse is over water
	static inline bool mouseOverWater = false;

	static inline std::unordered_map<std::string, GLuint> cursorLookup{
		{ "cursor",  GLFW_ARROW_CURSOR },
		{ "cursor1", GLFW_POINTING_HAND_CURSOR },
		{ "cursor2", GLFW_POINTING_HAND_CURSOR },
		{ "cursor3", GLFW_POINTING_HAND_CURSOR },
		{ "cursor4", GLFW_POINTING_HAND_CURSOR },
	};
};