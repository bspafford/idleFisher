#pragma once

#include <string>

class GLFWcursor;

class Cursor {
public:
	static void calcMouseImg();
	static void setMouseImg(std::string cursorName);

	static bool getMouseOverWater();
	static void setMouseOverWater(bool overWater);

	static void toggleCursor();

private:
	static inline GLFWcursor* cursor;
	static inline std::string currCursor;

	// if mouse is over water
	static inline bool mouseOverWater = false;
};