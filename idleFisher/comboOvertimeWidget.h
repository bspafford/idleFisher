#pragma once

#include "widget.h"

class UprogressBar;
class timer;

class UcomboOvertimeWidget : public widget {
public:
	UcomboOvertimeWidget();
	~UcomboOvertimeWidget();
	void draw(Shader* shaderProgram);
	void addTimer(timer* timer);
private:
	void updateProgressBar();
	void setupLocs() override;

	std::unique_ptr<UprogressBar> progressBar;
	timer* comboTimer;
};
