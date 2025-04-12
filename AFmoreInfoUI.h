#pragma once

#include "widget.h"

class AautoFisher;
class Ubutton;
class text;
struct Fimg;

class AFmoreInfoUI : public widget {
public:
	AFmoreInfoUI(AautoFisher* autoFisher);
	~AFmoreInfoUI();

	void draw(SDL_Renderer* renderer) override;

	void openUI();
	void closeUI();

	void addToViewport(bool override) override;

	void updateUI();

	void setupLocs() override;

private:
	AautoFisher* autoFisher;

	Fimg* background;
	Fimg* fullnessBar;
	Fimg* levelBar;

	text* fisherNum;
	text* levelText;
	text* fullnessText;
	text* fpsText;
	text* mpsText;

	Ubutton* closeButton;
};