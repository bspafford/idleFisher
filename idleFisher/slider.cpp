#include "slider.h"
#include "Input.h"
#include "text.h"
#include "Rectangle.h"

#include "debugger.h"

Uslider::Uslider(widget* parent, bool useCharLoc, vector size, float minVal, float maxVal, bool showValueText) : widget(parent) {
	this->size = size;
	this->useCharLoc = useCharLoc;
	this->minVal = minVal;
	this->maxVal = maxVal;
	defaultVal = minVal;
	value = &defaultVal;

	sliderTitle = std::make_unique<text>(this, " ", "straight", vector{ 0, 0 });
	sliderValueText = std::make_unique<text>(this, std::to_string(int(*value)), "straight", vector{0, 0});

	background = std::make_unique<URectangle>(vector{ 0, 0 }, this->size, useCharLoc, glm::vec4(0, 0, 0, 1));
	foreground = std::make_unique<URectangle>(vector{ 0, 0 }, this->size, useCharLoc);
	handle = std::make_unique<URectangle>(vector{ 0, 0 }, vector{ 5, 5 } * stuff::pixelSize, useCharLoc, glm::vec4(0, 0, 1, 1));
}

Uslider::~Uslider() {

}

void Uslider::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	sliderTitle->draw(shaderProgram);
	background->draw(shaderProgram);
	foreground->draw(shaderProgram);

	// clamp to edges
	vector halfHandleSize = handle->getSize() / 2.f;
	vector backgroundLoc = background->getLoc();
	vector backgroundSize = background->getSize();
	float min = backgroundLoc.x - halfHandleSize.x;
	float max = backgroundLoc.x + backgroundSize.x - halfHandleSize.x;

	if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_1) && mouseOver())
		sliding = true;
	else if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_1))
		sliding = false;

	if (sliding && Input::getMouseButtonHeld(GLFW_MOUSE_BUTTON_1))
		handle->setLoc({ math::clamp(Input::getMousePos().x, min, max), backgroundLoc.y + backgroundSize.y / 2.f - halfHandleSize.y});
	handle->draw(shaderProgram);

	float percent = (handle->getLoc().x + halfHandleSize.x - backgroundLoc.x) / backgroundSize.x;
	*value = (maxVal - minVal) * percent + minVal;

	sliderValueText->setText(std::to_string(int(*value)));
	sliderValueText->setLoc(backgroundLoc + vector{ backgroundSize.x + handle->getSize().x, backgroundSize.y / 2.f - sliderValueText->getSize().y / 2.f });

	sliderValueText->draw(shaderProgram);
}

void Uslider::setSliderTitle(std::string title) {
	sliderTitle->setText(title);
}

void Uslider::bindValue(float* value) {
	this->value = value;
}

void Uslider::setLoc(vector loc) {
	__super::setLoc(loc);
	sliderTitle->setLoc(loc);

	vector sliderSize = sliderTitle->getSize();
	background->setLoc(loc + vector{sliderSize.x, sliderSize.y / 2.f - background->getSize().y / 2.f});
	foreground->setLoc(background->getLoc());

	float percent = math::clamp((*value - minVal) / (maxVal - minVal), 0, 1);
	vector halfHandleSize = handle->getSize() / 2.f;
	handle->setLoc({ background->getLoc().x + background->getSize().x * percent - halfHandleSize.x, background->getLoc().y + (background->getSize().y - handle->getSize().y) / 2.f});
}

void Uslider::setSize(vector size) {

}

void Uslider::setBackgroundColor(glm::vec4 color) {
	background->setColor(color);

}

void Uslider::setForegroundColor(glm::vec4 color) {
	foreground->setColor(color);
}

bool Uslider::mouseOver() {
	vector mousePos = Input::getMousePos();

	vector handleSize = handle->getSize();
	vector loc = vector{ background->getLoc().x - handleSize.x / 2.f, getLoc().y };
	vector size = vector{ background->getSize().x + handleSize.x, handleSize.y };
	if (mousePos.x >= loc.x && mousePos.x <= loc.x + size.x && mousePos.y >= loc.y && mousePos.y <= loc.y + size.y)
		return true;
	return false;
}