#pragma once

#include "widget.h"

class text;
class timer;

class UnewRecordWidget : public widget {
public:
	UnewRecordWidget();
	void draw(Shader* shaderProgram);
	void start(int fishSize);
	void finished();

private:
	std::unique_ptr<text> recordText;
	std::unique_ptr<timer> startTimer;
};