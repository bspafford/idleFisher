#pragma once

#include "widget.h"

struct Image;
class Ubutton;
class text;
class verticalBox;
class Usettings;

class UpauseMenu : public widget {
public:
	UpauseMenu(widget* parent);
	~UpauseMenu();
	void draw(Shader* shaderProgram);
private:
	void setupLocs() override;

	void resume();
	void saveGame();
	void settings();
	void exitToMenu();
	void exitToDesktop();

	std::unique_ptr<Image> pauseText;
	std::unique_ptr<Ubutton> resumeButton;
	std::unique_ptr<Ubutton> saveGameButton;
	std::unique_ptr<Ubutton> settingsButton;
	std::unique_ptr<Ubutton> exitToMenuButton;
	std::unique_ptr<Ubutton> exitToDesktopButton;
	std::unique_ptr<verticalBox> vertBox;

	std::unique_ptr<Usettings> settingsWidget;
};