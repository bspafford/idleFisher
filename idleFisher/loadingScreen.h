#pragma once

#include "widget.h"

class timer;
class text;

class LoadingScreen : public widget {
public:
	LoadingScreen(widget* parent);

	void draw(Shader* shaderProgram) override;
	void setupLocs() override;

private:
	std::unique_ptr<text> loadingText;
	std::unique_ptr<timer> loadingTextTimer;
	void loadingTimerCallback();

	int periodNum = 0;
};