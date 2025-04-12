#include "slider.h"
#include "main.h"
#include "text.h"

Uslider::Uslider(bool useCharLoc, vector size, float minVal, float maxVal, bool showValueText) {
	this->size = size * stuff::pixelSize;
	this->useCharLoc = useCharLoc;
	this->minVal = minVal;
	this->maxVal = maxVal;
	value = new float(minVal);

	sliderTitle = new text(" ", "straight", { 0, 0 });
	sliderValueText = new text(std::to_string(int(*value)), "straight", {0, 0}, false, false, textAlign::right);
}

Uslider::~Uslider() {

}

void Uslider::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	sliderTitle->draw(renderer);
	vector newLoc = loc + vector{ sliderTitle->getSize().x + 5 * stuff::pixelSize, 0 };
	if (useCharLoc)
		newLoc = math::worldToScreen(newLoc);
	vector newSize = size - vector{ sliderTitle->getSize().x + 5 * stuff::pixelSize, 0 };

	float percentVal = *value / maxVal;

	SDL_Rect background = { int(newLoc.x), int(newLoc.y), int(newSize.x), int(newSize.y) };
	SDL_Rect foreground = { int(newLoc.x), int(newLoc.y), int(newSize.x * percentVal), int(newSize.y) };
	int handleSize = newSize.y + stuff::pixelSize;
	SDL_Rect handle = { int(newLoc.x + newSize.x * percentVal - handleSize / 2), int(newLoc.y + newSize.y / 2 - handleSize / 2), handleSize, handleSize };
	if (!prevMouseDown && Main::bLeftMouseButtonDown && mouseOver()) {
		mouseDown = true;
	} else if (!Main::bLeftMouseButtonDown) {
		mouseDown = false;
	}

	if (mouseDown) {
		// clamp value
		float min = background.x - handleSize / 2;
		float max = background.x + background.w - handleSize / 2;
		handle.x = math::clamp(Main::mousePos.x - parent->getLoc().x, min, max);

		// update value
		float percent =  (handle.x - min) / (max - min);

		float prevVal = *value;
		*value = (maxVal - minVal) * percent + minVal;
		if (sliderValueText && prevVal != *value) // updates text if value has changed
			sliderValueText->setText(std::to_string(int(*value)));
	}

	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_RenderFillRect(renderer, &background);
	SDL_SetRenderDrawColor(renderer, foregroundColor.r, foregroundColor.g, foregroundColor.b, foregroundColor.a);
	SDL_RenderFillRect(renderer, &foreground);
	SDL_RenderFillRect(renderer, &handle);

	sliderValueText->setLoc({ float(size.x + 20 * stuff::pixelSize), float(background.y + background.h / 2 - sliderValueText->getSize().y / 2) });
	sliderValueText->draw(renderer);

	prevMouseDown = Main::bLeftMouseButtonDown;
}

void Uslider::setSliderTitle(std::string title) {
	sliderTitle->setText(title);
}

void Uslider::bindValue(float* value) {
	this->value = value;

	sliderValueText->setText(std::to_string(int(*value)));
}

void Uslider::setLoc(vector loc) {
	__super::setLoc(loc);
	sliderTitle->setLoc(loc);
}

void Uslider::setSize(vector size) {

}

void Uslider::setBackgroundColor(SDL_Color color) {
	backgroundColor = color;
}

void Uslider::setForegroundColor(SDL_Color color) {
	foregroundColor = color;
}