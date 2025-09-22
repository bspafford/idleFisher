#pragma once

#include "widget.h"

class AautoFisher;
class Ubutton;
class text;

class AFmoreInfoUI : public widget {
public:
	AFmoreInfoUI(AautoFisher* autoFisher);
	~AFmoreInfoUI();

	void draw(Shader* shaderProgram) override;

	void openUI();
	void closeUI();

	void addToViewport(bool override) override;

	void updateUI();

	void setupLocs() override;

private:
	AautoFisher* autoFisher;

	Image* background;
	Image* fullnessBar;
	Image* levelBar;

	text* fisherNum;
	text* levelText;
	text* fullnessText;
	text* fpsText;
	text* mpsText;

	Ubutton* closeButton;
};