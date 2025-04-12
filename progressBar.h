#pragma once

#include "widget.h"

struct FprogressBarDir {
	static inline int left = 0;
	static inline int right = 1;
	static inline int up = 2;
	static inline int down = 3;
};

class UprogressBar : public widget {
public:
	UprogressBar(bool useCharLoc, float width, float height, int dir = FprogressBarDir::right, bool reverse = false);
	// background color, foreground color, direction (left, up, reverse), width, height
	void setBackgroundColor(SDL_Color color);
	// id is when the progress bar has mutliple preogress bars
	void setForegroundColor(SDL_Color color, int id = 0);
	void draw(SDL_Renderer* renderer);
	// id is when the progress bar has mutliple preogress bars
	void setPercent(float percent, int id = 0);
	// if this is the first time adding, make sure to update the weight of the first progress bar
	void addProgressBar(float percent, float weight, SDL_Color color);

	float calcTotalWeight();

private:
	std::vector<float> percent = { 0 };
	std::vector<float> weight = { 1 };
	bool useCharLoc;
	int dir;
	bool reverse;
	std::vector<SDL_Color> foregroundColor = std::vector<SDL_Color>{ { 255, 255, 255, 255 } };
	SDL_Color backgroundColor = { 0, 0, 0, 255 };
};
