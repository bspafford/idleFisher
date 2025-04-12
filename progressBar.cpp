#include "progressBar.h"

UprogressBar::UprogressBar(bool useCharLoc, float width, float height, int dir, bool reverse) {
	this->useCharLoc = useCharLoc;

	size = vector{ width, height } * stuff::pixelSize;
	this->dir = dir;
	this->reverse = reverse;
}

void UprogressBar::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	vector newLoc = loc;
	if (useCharLoc)
		newLoc = math::worldToScreen(loc);


	SDL_Rect background = { int(newLoc.x), int(newLoc.y), int(size.x), int(size.y) };
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_RenderFillRect(renderer, &background);
	std::vector<SDL_Rect> foreground = std::vector<SDL_Rect>(percent.size());
	float totalWeight = calcTotalWeight();
	for (int i = 0; i < percent.size(); i++) {
		float percentVal = reverse ? 1 - percent[i] : percent[i];
		if (dir == FprogressBarDir::left) {
			int temp = round(size.x * percentVal);
			foreground[0] = { int(newLoc.x + size.x - temp), int(newLoc.y), int(temp), int(size.y) };
		} else if (dir == FprogressBarDir::right) {
			// foreground = { int(newLoc.x), int(newLoc.y), int(size.x * percentVal), int(size.y) };
			foreground[i] = { int(newLoc.x), int(newLoc.y), int(size.x * percentVal * (weight[i] / totalWeight)), int(size.y) };
			newLoc.x += size.x * percentVal * (weight[i] / totalWeight);

		} else if (dir == FprogressBarDir::up) {
			int temp = round(size.y * percentVal);
			foreground[0] = { int(newLoc.x), int(newLoc.y + size.y - temp), int(size.x), int(temp) };
		} else if (dir == FprogressBarDir::down)
			foreground[0] = { int(newLoc.x), int(newLoc.y), int(size.x), int(size.y * percentVal) };

		SDL_SetRenderDrawColor(renderer, foregroundColor[i].r, foregroundColor[i].g, foregroundColor[i].b, foregroundColor[i].a);
		SDL_RenderFillRect(renderer, &foreground[i]);
	}
}

// background color, foreground color, direction (left, up, reverse), width, height
void UprogressBar::setBackgroundColor(SDL_Color color) {
	backgroundColor = color;
}

void UprogressBar::setForegroundColor(SDL_Color color, int id) {
	foregroundColor[id] = color;
}

void UprogressBar::setPercent(float percent, int id) {
	this->percent[id] = math::clamp(percent, 0, 1);
}

void UprogressBar::addProgressBar(float percent, float weight, SDL_Color color) {
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
