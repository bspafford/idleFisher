#pragma once

#include "widget.h"

class UprogressBar;
class timer;

class UcomboOvertimeWidget : public widget {
public:
	UcomboOvertimeWidget();
	~UcomboOvertimeWidget();
	void draw(SDL_Renderer* renderer);
	void addTimer(timer* timer);
private:
	void updateProgressBar();
	void setupLocs() override;

	UprogressBar* progressBar;
	timer* comboTimer;
};
