#include "slider.h"
#include "main.h"
#include "text.h"
#include "Rectangle.h"

Uslider::Uslider(bool useCharLoc, vector size, float minVal, float maxVal, bool showValueText) {
	this->size = size * stuff::pixelSize;
	this->useCharLoc = useCharLoc;
	this->minVal = minVal;
	this->maxVal = maxVal;
	value = &minVal;

	sliderTitle = std::make_unique<text>(" ", "straight", vector{ 0, 0 });
	sliderValueText = std::make_unique<text>(std::to_string(int(*value)), "straight", vector{0, 0}, false, false, textAlign::right);

	background = std::make_unique<URectangle>(vector{ 0, 0 }, size, useCharLoc);
	foreground = std::make_unique<URectangle>(vector{ 0, 0 }, size, useCharLoc);
	handle = std::make_unique<URectangle>(vector{ 0, 0 }, vector{ 3, 3 }, useCharLoc);
}

Uslider::~Uslider() {

}

void Uslider::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	sliderTitle->draw(shaderProgram);
	vector newLoc = loc + vector{ sliderTitle->getSize().x + 5 * stuff::pixelSize, 0 };
	if (useCharLoc)
		newLoc = math::worldToScreen(newLoc, "topleft");
	vector newSize = size - vector{ sliderTitle->getSize().x + 5 * stuff::pixelSize, 0 };

	float percentVal = *value / maxVal;

	int handleSize = newSize.y + stuff::pixelSize;
	Rect handleRect = { int(newLoc.x + newSize.x * percentVal - handleSize / 2), int(newLoc.y + newSize.y / 2 - handleSize / 2), handleSize, handleSize };

	if (!prevMouseDown && Main::bLeftMouseButtonDown && mouseOver()) {
		mouseDown = true;
	} else if (!Main::bLeftMouseButtonDown) {
		mouseDown = false;
	}

	vector backgroundSize = background->getSize();
	vector backgroundLoc = background->getLoc();

	if (mouseDown) {
		// clamp value
		float min = backgroundLoc.x - handleSize / 2;
		float max = backgroundLoc.x + backgroundSize.x - handleSize / 2;
		handleRect.x = math::clamp(Main::mousePos.x - parent->getLoc().x, min, max);

		// update value
		float percent =  (handleRect.x - min) / (max - min);

		float prevVal = *value;
		*value = (maxVal - minVal) * percent + minVal;
		if (sliderValueText && prevVal != *value) // updates text if value has changed
			sliderValueText->setText(std::to_string(int(*value)));
	}

	background->draw(shaderProgram);
	foreground->draw(shaderProgram);
	handle->draw(shaderProgram);

	sliderValueText->setLoc({ float(size.x + 20 * stuff::pixelSize), float(backgroundLoc.y + backgroundSize.y / 2 - sliderValueText->getSize().y / 2) });
	sliderValueText->draw(shaderProgram);

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

	background->setLoc(loc);
	foreground->setLoc(loc);
}

void Uslider::setSize(vector size) {

}

void Uslider::setBackgroundColor(glm::vec4 color) {
	background->setColor(color);

}

void Uslider::setForegroundColor(glm::vec4 color) {
	foreground->setColor(color);
}