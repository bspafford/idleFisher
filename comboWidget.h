#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "widget.h"

class text;
struct Fimg;

class UcomboWidget : public widget {
public:
	UcomboWidget();
	~UcomboWidget();

	void setupLocs() override;

	void tempIncrease();
	void tempSame();
	void tempBreak();

	void spawnComboNumber(int comboNum);

	int prevComboNum = 1;

	void showComboText();
	void hideComboText();

	bool fading = false;
	float fadeTimerMax = .4f;
	float fadeTimer = 0;

	void update(float deltaTime); // native tick for some reason makes play animation not work
	void draw(SDL_Renderer* renderer);

	text* comboText;
};