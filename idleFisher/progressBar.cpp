#include "progressBar.h"

UprogressBar::UprogressBar(bool useCharLoc, float width, float height, int dir, bool reverse) {
	this->useCharLoc = useCharLoc;

	size = vector{ width, height } * stuff::pixelSize;
	this->dir = dir;
	this->reverse = reverse;
}

void UprogressBar::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	vector newLoc = loc;
	background = std::make_unique<URectangle>(newLoc, size, useCharLoc, backgroundColor);
	background->draw(shaderProgram);

	std::vector<std::unique_ptr<URectangle>> foreground = std::vector<std::unique_ptr<URectangle>>(percent.size());
	float totalWeight = calcTotalWeight();
	for (int i = 0; i < percent.size(); i++) {
		float percentVal = reverse ? 1 - percent[i] : percent[i];
		if (dir == FprogressBarDir::left) {
			int temp = round(size.x * percentVal);
			foreground[0] = std::make_unique<URectangle>(vector{newLoc.x + size.x - temp, newLoc.y}, vector{float(temp), size.y}, useCharLoc, foregroundColor[i]);
		} else if (dir == FprogressBarDir::right) {
			foreground[i] = std::make_unique<URectangle>(newLoc, vector{size.x * percentVal * (weight[i] / totalWeight), size.y }, useCharLoc, foregroundColor[i]);
			newLoc.x += size.x * percentVal * (weight[i] / totalWeight);

		} else if (dir == FprogressBarDir::up) {
			int temp = round(size.y * percentVal);
			foreground[0] = std::make_unique<URectangle>(vector{ newLoc.x, newLoc.y + size.y - temp }, vector{ size.x, float(temp) }, useCharLoc, foregroundColor[i]);
		} else if (dir == FprogressBarDir::down)
			foreground[0] = std::make_unique<URectangle>(newLoc, vector{ size.x, size.y * percentVal }, useCharLoc, foregroundColor[i]);

		foreground[i]->draw(shaderProgram);
	}
}

// background color, foreground color, direction (left, up, reverse), width, height
void UprogressBar::setBackgroundColor(glm::vec4 color) {
	backgroundColor = color;
}

void UprogressBar::setForegroundColor(glm::vec4 color, int id) {
	foregroundColor[id] = color;
}

void UprogressBar::setPercent(float percent, int id) {
	this->percent[id] = math::clamp(percent, 0, 1);
}

void UprogressBar::addProgressBar(float percent, float weight, glm::vec4 color) {
	this->percent.push_back(percent);
	foregroundColor.push_back(color);
	this->weight.push_back(weight);
}

float UprogressBar::calcTotalWeight() {
	float total = 0;
	for (float w : weight)
		total += w;
	return total;
}
