#pragma once

#include "widget.h"

class Ubutton;
class text;
class verticalBox;
struct Fimg;


class UpauseMenu : public widget {
public:
	UpauseMenu();
	~UpauseMenu();
	void draw(SDL_Renderer* renderer);
private:
	void setupLocs() override;

	void resume();
	void saveGame();
	void settings();
	void exitToMenu();
	void exitToDesktop();

	Fimg* pauseText;
	Ubutton* resumeButton;
	Ubutton* saveGameButton;
	Ubutton* settingsButton;
	Ubutton* exitToMenuButton;
	Ubutton* exitToDesktopButton;
	verticalBox* vertBox;
};