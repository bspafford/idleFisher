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


	void draw(Shader* shaderProgram) override;

	Image* mapBackground;

private:
	void addedToViewport() override;
	void closeWidget();

	Umap* map;
	animation* mapAnim;
	Image* mapClosed;

	Ubutton* xButton;

	bool mouseDown = false;

	vector mouseStartPos;
	vector imgStartPos;

	void setupLocs() override;

	timer* mapTimer;
	void moveAnim();
	void mapTimerFinish();
	void mapAnimFinish();
	void removeFromViewport() override;

	bool opening = false;
	bool closing = false;
	// if journal is all the way open
	bool open = false;
};
