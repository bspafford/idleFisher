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

	std::unique_ptr<Image> background;

	std::unique_ptr<timer> changeTextTimer;

	std::unique_ptr<text> startTimeText;
	std::unique_ptr<UequippedWidget> equippedWidget;
	std::unique_ptr<UscrollBox> scrollBox;
	std::unique_ptr<text> achievementText;
	std::unique_ptr<UwrapBox> achievementHolder;
	std::unique_ptr<UhoverBox> hoverBox;
	std::unique_ptr<Ubutton> xButton;
};