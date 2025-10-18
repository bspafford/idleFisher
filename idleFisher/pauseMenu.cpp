#include "pauseMenu.h"
#include "main.h"
#include "saveData.h"
#include "math.h"
#include "settings.h"
#include "button.h"
#include "text.h"
#include "verticalBox.h"

UpauseMenu::UpauseMenu(widget* parent) : widget(parent) {
	pauseText = std::make_unique<Image>("./images/widget/pauseMenu/pause.png", vector{ 3, 3 } * stuff::pixelSize, false);
	pauseText->setAnchor(anchor::left, anchor::top);
 	resumeButton = std::make_unique<Ubutton>(this, "widget/pauseMenu/continue.png", 67, 19, 1, vector{ 0, 0 }, false, false);
	saveGameButton = std::make_unique<Ubutton>(this, "widget/pauseMenu/saveGame.png", 70, 19, 1, vector{ 0, 0 }, false, false);
	settingsButton = std::make_unique<Ubutton>(this, "widget/pauseMenu/settings.png", 68, 19, 1, vector{ 0, 0 }, false, false);
	exitToMenuButton = std::make_unique<Ubutton>(this, "widget/pauseMenu/exitToMenu.png", 91, 19, 1, vector{ 0, 0 }, false, false);
	exitToDesktopButton = std::make_unique<Ubutton>(this, "widget/pauseMenu/exitGame.png", 72, 19, 1, vector{ 0, 0 }, false, false);
	
	vertBox = std::make_unique<verticalBox>(this);
	if (vertBox) {
		float padding = 10 * stuff::pixelSize;
		if (resumeButton) {
			resumeButton->addCallback(this, &UpauseMenu::resume);
			vertBox->addChild(resumeButton.get(), resumeButton->getSize().y + padding);
		}
		if (saveGameButton) {
			saveGameButton->addCallback(this, &UpauseMenu::saveGame);
			vertBox->addChild(saveGameButton.get(), saveGameButton->getSize().y + padding);
		}
		if (settingsButton) {
			settingsButton->addCallback(this, &UpauseMenu::settings);
			vertBox->addChild(settingsButton.get(), settingsButton->getSize().y + padding);
		}
		if (exitToMenuButton) {
			exitToMenuButton->addCallback(this, &UpauseMenu::exitToMenu);
			vertBox->addChild(exitToMenuButton.get(), exitToMenuButton->getSize().y + padding);
		}
		if (exitToDesktopButton) {
			exitToDesktopButton->addCallback(this, &UpauseMenu::exitToDesktop);
			vertBox->addChild(exitToDesktopButton.get(), exitToDesktopButton->getSize().y + padding);
		}
	}

	setupLocs();
}

UpauseMenu::~UpauseMenu() {

}

void UpauseMenu::draw(Shader* shaderProgram) {
	pauseText->draw(shaderProgram);
	vertBox->draw(shaderProgram);
}

void UpauseMenu::setupLocs() {
	vertBox->getSize();
	vertBox->setLocAndSize({ 8 * stuff::pixelSize, stuff::screenSize.y * .25f }, stuff::screenSize);
}

void UpauseMenu::resume() {
	Main::currWidget->removeFromViewport();
}

void UpauseMenu::saveGame() {
	SaveData::save();
}

void UpauseMenu::settings() {
	Main::settingsWidget->addToViewport(true);
	Main::settingsWidget->setParent(this);
}

void UpauseMenu::exitToMenu() {
	Main::openLevel("titleScreen");
}

void UpauseMenu::exitToDesktop() {
	glfwSetWindowShouldClose(Main::window, true);
}