#pragma once

#include "widget.h"

class Ubutton;
class text;
class UrebirthUnlock;
class UhoverBox;

class UrebirthWidget : public widget {
public:
	UrebirthWidget();
	~UrebirthWidget();

	void draw(Shader* shaderProgram) override;
	void update();

private:
	bool isMouseOverUnlock();
	void scrolling();
	void setupLocs() override;
	void addedToViewport() override;
	void drawLine(Shader* shaderProgram, vector loc1, vector loc2);

	void openRebirthWorld();

	bool mouseDownPrev = false;
	vector scrollLoc;
	vector startLoc;
	vector mouseStartPos;

	std::vector<std::unique_ptr<UrebirthUnlock>> rebirthButtonList;

	std::unique_ptr<Ubutton> xButton;
	std::unique_ptr<Ubutton> rebirthButton;
	std::unique_ptr<text> rebirthText;
	std::unique_ptr<text> rebirthNum;

	bool wasOverButton = false;
	bool justOpened = true;

	std::unique_ptr<UhoverBox> hoverBox;
};