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

	std::unique_ptr<Image> pauseText;
	std::unique_ptr<Ubutton> resumeButton;
	std::unique_ptr<Ubutton> saveGameButton;
	std::unique_ptr<Ubutton> settingsButton;
	std::unique_ptr<Ubutton> exitToMenuButton;
	std::unique_ptr<Ubutton> exitToDesktopButton;
	std::unique_ptr<verticalBox> vertBox;
};