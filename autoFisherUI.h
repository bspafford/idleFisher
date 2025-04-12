#pragma once

#include "widget.h"

#include <SDL.h>

class AFmoreInfoUI;
class animation;
class Ubutton;
class text;
struct Fimg;

class autoFisherUI : public widget {
public:
	autoFisherUI(class AautoFisher* autoFisherRef, vector loc);
	~autoFisherUI();

	void draw(SDL_Renderer* renderer);

	void updateUI();
	void openUI();
	void closeUI();

	bool visible = false;

	bool opening, closing;

private:
	void upgrade();

	void openedUI();
	void closedUI();

	void moveButtonText(int frame);
	vector buttonTextLoc;

	bool UIOpen;

	AautoFisher* autoFisher;

	Ubutton* upgradeButton;
	// Fimg fullness;
	//Fimg multiplierButton;
	Ubutton* multi1;
	Ubutton* multi10;
	Ubutton* multiMax;
	Fimg* pressTest;

	Ubutton* menuButton;
	void openMenu();

	Fimg* level;

	// text* fullnessText;
	text* levelText;
	text* maxText; // text show when player upgrades multi is MAX
public:
	text* buttonText;
private:
	std::vector<std::string> levelBarImgs;
	// std::vector<std::string> fullnessBarImgs;

	vector UILoc;

	animation* openAnimation;

	void setMultiplier(int multiplier);
	void setMulti1();
	void setMulti10();
	void setMultiMax();

	std::vector<std::string> multi1Anim;
};