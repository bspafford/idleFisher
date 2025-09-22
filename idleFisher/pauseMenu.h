#pragma once

#include "widget.h"

class Ubutton;
class text;
class verticalBox;
struct Image;


class UpauseMenu : public widget {
public:
	UpauseMenu();
	~UpauseMenu();
	void draw(Shader* shaderProgram);
private:
	void setupLocs() override;

	void resume();
	void saveGame();
	void settings();
	void exitToMenu();
	void exitToDesktop();

	Image* pauseText;
	Ubutton* resumeButton;
	Ubutton* saveGameButton;
	Ubutton* settingsButton;
	Ubutton* exitToMenuButton;
	Ubutton* exitToDesktopButton;
	verticalBox* vertBox;
};