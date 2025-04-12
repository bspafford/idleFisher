#pragma once

#include "widget.h"

class text;
class Ubutton;
class Umap;
class animation;
class timer;

class UsailorWidget : public widget {
public:
	UsailorWidget();
	~UsailorWidget();


	void draw(SDL_Renderer* renderer) override;

	Fimg* mapBackground;

private:
	void addedToViewport() override;
	void closeWidget();

	Umap* map;
	animation* mapOpenAnim;
	Fimg* mapClosed;

	timer* mapTimer;
	void moveAnim();
	void finishmapAnim();

	Ubutton* xButton;

	bool mouseDown = false;

	vector mouseStartPos;
	vector imgStartPos;

	void setupLocs() override;

	SDL_Texture* test;
};
