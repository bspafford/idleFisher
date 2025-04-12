#pragma once

#include "widget.h"

class text;
class timer;

class UnewRecordWidget : public widget {
public:
	UnewRecordWidget();
	void draw(SDL_Renderer* renderer);
	void start(int fishSize);
	void finished();

private:
	text* recordText;
	timer* startTimer;
};