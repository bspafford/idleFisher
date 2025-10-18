#pragma once

#include "widget.h"
#include "Rectangle.h"

struct FprogressBarDir {
	static inline int left = 0;
	static inline int right = 1;
	static inline int up = 2;
	static inline int down = 3;
};

class UprogressBar : public widget {
public:
	UprogressBar(widget* parent, bool useCharLoc, float width, float height, int dir = FprogressBarDir::right, bool reverse = false);
	// background color, foreground color, direction (left, up, reverse), width, height
	void setBackgroundColor(glm::vec4 color);
	// id is when the progress bar has mutliple preogress bars
	void setForegroundColor(glm::vec4 color, int id = 0);
	void draw(Shader* shaderProgram);
	// id is when the progress bar has mutliple preogress bars
	void setPercent(float percent, int id = 0);
	// if this is the first time adding, make sure to update the weight of the first progress bar
	void addProgressBar(float percent, float weight, glm::vec4 color);

	float calcTotalWeight();

private:
	std::vector<float> percent = { 0 };
	std::vector<float> weight = { 1 };
	bool useCharLoc;
	int dir;
	bool reverse;
	std::vector<glm::vec4> foregroundColor = std::vector<glm::vec4>{ { 255, 255, 255, 255 } };
	glm::vec4 backgroundColor = { 0, 0, 0, 255 };

	std::unique_ptr<URectangle> foreground;
	std::unique_ptr<URectangle> background;
};
