#include "pauseMenu.h"
#include "main.h"
#include "saveData.h"
#include "math.h"
#include "settings.h"
#include "button.h"
#include "text.h"
#include "verticalBox.h"

UpauseMenu::UpauseMenu() {
	pauseText = new Fimg("./images/widget/pauseMenu/pause.png", vector{ 3, 3 } * stuff::pixelSize);
	resumeButton = new Ubutton(this, "widget/pauseMenu/continue.png", 67, 19, 1, { 0, 0 }, false, false);
	saveGameButton = new Ubutton(this, "widget/pauseMenu/saveGame.png", 70, 19, 1, { 0, 0 }, false, false);
	settingsButton = new Ubutton(this, "widget/pauseMenu/settings.png", 68, 19, 1, { 0, 0 }, false, false);
	exitToMenuButton = new Ubutton(this, "widget/pauseMenu/exitToMenu.png", 91, 19, 1, { 0, 0 }, false, false);
	exitToDesktopButton = new Ubutton(this, "widget/pauseMenu/exitGame.png", 72, 19, 1, { 0, 0 }, false, false);
	
	vertBox = new verticalBox();
	if (vertBox) {
		float padding = 10 * stuff::pixelSize;
		if (resumeButton) {
			resumeButton->addCallback(this, &UpauseMenu::resume);
			vertBox->addChild(resumeButton, resumeButton->getSize().y + padding);
		}
		if (saveGameButton) {
			saveGameButton->addCallback(this, &UpauseMenu::saveGame);
			vertBox->addChild(saveGameButton, saveGameButton->getSize().y + padding);
		}
		if (settingsButton) {
			settingsButton->addCallback(this, &UpauseMenu::settings);
			vertBox->addChild(settingsButton, settingsButton->getSize().y + padding);
		}
		if (exitToMenuButton) {
			exitToMenuButton->addCallback(this, &UpauseMenu::exitToMenu);
			vertBox->addChild(exitToMenuButton, exitToMenuButton->getSize().y + padding);
		}
		if (exitToDesktopButton) {
			exitToDesktopButton->addCallback(this, &UpauseMenu::exitToDesktop);
			vertBox->addChild(exitToDesktopButton, exitToDesktopButton->getSize().y + padding);
		}
	}

	setupLocs();
}

UpauseMenu::~UpauseMenu() {
	delete vertBox;
	vertBox = nullptr;
}

void UpauseMenu::draw(SDL_Renderer* renderer) {
	pauseText->draw(renderer, false);
	vertBox->draw(renderer);
}

void UpauseMenu::setupLocs() {
	vertBox->getSize();
	vertBox->setLocAndSize({ 8 * stuff::pixelSize, Main::screenHeight * .25f }, { float(Main::screenWidth), float(Main::screenHeight) });
}

void UpauseMenu::resume() {
	Main::currWidget->removeFromViewport();
}

void UpauseMenu::saveGame() {
	SaveData::save();
}

void UpauseMenu::settings() {
	Main::settingsWidget->addToViewport(true);
	Main::settingsWidget->parent = this;
}

void UpauseMenu::exitToMenu() {
	Main::openLevel("titleScreen");
}

void UpauseMenu::exitToDesktop() {
	Main::running = false;
}