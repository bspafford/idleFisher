#pragma once

#include "widget.h"

class timer;
class text;
class Ubutton;
class UhoverBox;
class UscrollBox;
class UwrapBox;
class UequippedWidget;

class UachievementWidget : public widget {
public:
	UachievementWidget();
	~UachievementWidget();

	void draw(Shader* shaderProgram) override;

	void updateEquipmentWidget();
	void updateAchievementIcon(int id);
private:
	void setupLocs() override;
	void closeWidget();
	void updateText();
	void addedToViewport() override;

	Image* background;

	timer* changeTextTimer;

	text* startTimeText;
	UequippedWidget* equippedWidget;
	UscrollBox* scrollBox;
	text* achievementText;
	UwrapBox* achievementHolder;
	UhoverBox* hoverBox;
	Ubutton* xButton;
};