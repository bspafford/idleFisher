#pragma once

#include "widget.h"

class text;

class Uslider : public widget {
public:
	Uslider(bool useCharLoc, vector size, float minVal, float maxVal, bool showValueText = true);
	~Uslider();
	void setBackgroundColor(glm::vec4 color);
	void setForegroundColor(glm::vec4 color);
	void draw(Shader* shaderProgram) override;
	void setLoc(vector loc) override;
	void setSize(vector size) override;
	void setSliderTitle(std::string title); // gives a title on the left size of the progress bar
	void bindValue(float* value);
private:
	text* sliderTitle;
	text* sliderValueText; // text to right of slider 0-100

	bool useCharLoc;
	float* value;
	float minVal;
	float maxVal;
	URectangle* foreground;
	URectangle* background;
	URectangle* handle;


	bool prevMouseDown;
	bool mouseDown = false;
};