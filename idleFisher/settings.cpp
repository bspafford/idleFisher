#include "settings.h"
#include "main.h"
#include "math.h"
#include "text.h"
#include "scrollBox.h"
#include "button.h"
#include "pauseMenu.h"
#include "slider.h"
#include "horizontalBox.h"
#include "saveData.h"

Usettings::Usettings() {
	SaveData::loadSettings();

	// button click anim
	std::vector<std::string> buttonClickAnim;
	for (int i = 0; i < 2; i++)
		buttonClickAnim.push_back("./images/widget/upgradeButton" + std::to_string(i + 1) + ".png");

	background = new Image("./images/widget/achievementBackground.png", { 0, 0 }, false);

	backButton = new Ubutton(this, "widget/npcXButton.png", 11, 11, 1, { 0, 0 }, false, false);
	backButton->addCallback(this, &Usettings::goBack);

	scrollBox = new UscrollBox();

	settingsTitle = new text("Settings", "biggerStraight", { 0, 0 });
	audioTitle = new text("  Audio:", "biggerStraight", { 0, 0 });
	graphicsTitle = new text("  Graphics:", "biggerStraight", { 0, 0 });

	// padding
	scrollBox->addChild(NULL, 3 * stuff::pixelSize);
// settings
	scrollBox->addChild(settingsTitle, settingsTitle->getSize().y + 3 * stuff::pixelSize);

	saveButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	saveButton->addCallback(SaveData::saveSettings);
	saveText = new text("Save", "straightDark", { 0, 0 });
	
	cancelButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	cancelButton->addCallback(this, &Usettings::cancel);
	
	cancelText = new text("Cancel", "straightDark", { 0, 0 });

// audio
	scrollBox->addChild(audioTitle, audioTitle->getSize().y + 3 * stuff::pixelSize);

	masterVolumeSlider = new Uslider(false, { background->w - 50.f, 3}, 0, 100);
	masterVolumeSlider->setSliderTitle("     Master ");
	masterVolumeSlider->bindValue(&SaveData::settingsData.masterVolume);
	musicVolume = new Uslider(false, { background->w - 50.f, 3 }, 0, 100);
	musicVolume->setSliderTitle("     Music   ");
	musicVolume->bindValue(&SaveData::settingsData.musicVolume);
	sfxVolume = new Uslider(false, { background->w - 50.f, 3 }, 0, 100);
	sfxVolume->setSliderTitle("     SFX       ");
	sfxVolume->bindValue(&SaveData::settingsData.sfxVolume);
	dialogVolume = new Uslider(false, { background->w - 50.f, 3 }, 0, 100);
	dialogVolume->setSliderTitle("     Dialog ");
	dialogVolume->bindValue(&SaveData::settingsData.dialogVolume);

	scrollBox->addChild(masterVolumeSlider, masterVolumeSlider->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(musicVolume, musicVolume->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(sfxVolume, sfxVolume->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(dialogVolume, dialogVolume->getSize().y + 5 * stuff::pixelSize);

// graphics
	scrollBox->addChild(graphicsTitle, graphicsTitle->getSize().y + 3 * stuff::pixelSize);

	pixelFontHorizBox;
	text* pixelFontText;
	Ubutton* pixelFontButton;
	pixelFontHorizBox = new horizontalBox();
	pixelFontText = new text("     Pixel Font", "straight", { 0, 0 });

	pixelFontButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	//pixelFontButton->setClickAnim(buttonClickAnim);
	pixelFontButton->addCallback(this, &Usettings::togglePixelText);
	pixelFontHorizBox->addChild(pixelFontText, pixelFontText->getSize().x);
	pixelFontHorizBox->addChild(NULL, 229 * stuff::pixelSize);
	pixelFontHorizBox->addChild(pixelFontButton, pixelFontButton->getSize().x);
	scrollBox->addChild(pixelFontHorizBox, 16 * stuff::pixelSize);

	shortNumHorizBox = new horizontalBox();
	shortNumbersText = new text("     Enable Short Numbers", "straight", { 0, 0 });
	shortNumbersButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	//shortNumbersButton->setClickAnim(buttonClickAnim);
	shortNumbersButton->addCallback(this, &Usettings::toggleShortNumbers);
	shortNumHorizBox->addChild(shortNumbersText, shortNumbersText->getSize().x);
	shortNumHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	shortNumHorizBox->addChild(shortNumbersButton, shortNumbersButton->getSize().x);
	scrollBox->addChild(shortNumHorizBox, 16 * stuff::pixelSize);

	// show pets
	petHorizBox = new horizontalBox();
	petText = new text("     Show Pets                     ", "straight", { 0, 0 });
	petButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	//petButton->setClickAnim(buttonClickAnim);
	petButton->addCallback(this, &Usettings::togglePets);
	petHorizBox->addChild(petText, petText->getSize().x);
	petHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	petHorizBox->addChild(petButton, petButton->getSize().x);
	scrollBox->addChild(petHorizBox, 16 * stuff::pixelSize);

	// show rain
	rainHorizBox = new horizontalBox();
	rainText = new text("     Show Rain                     ", "straight", { 0, 0 });
	rainButton = new Ubutton(this, "widget/upgradeButton.png", 35, 13, 1, { 0, 0 }, false, false);
	//rainButton->setClickAnim(buttonClickAnim);
	rainButton->addCallback(this, &Usettings::toggleRain);
	rainHorizBox->addChild(rainText, rainText->getSize().x);
	rainHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	rainHorizBox->addChild(rainButton, rainButton->getSize().x);
	scrollBox->addChild(rainHorizBox, 16 * stuff::pixelSize);


	setupText();

	setupLocs();
}

Usettings::~Usettings() {
	delete settingsTitle;
	delete audioTitle;
	delete graphicsTitle;
	settingsTitle = nullptr;
	audioTitle = nullptr;
	graphicsTitle = nullptr;
}

void Usettings::draw(Shader* shaderProgram) {
	background->draw(shaderProgram);
	backButton->draw(shaderProgram);

	scrollBox->draw(shaderProgram);

	if (saveButton)
		saveButton->draw(shaderProgram);
	saveText->draw(shaderProgram);
	if (cancelButton)
		cancelButton->draw(shaderProgram);
	cancelText->draw(shaderProgram);
}

void Usettings::goBack() {
	// go back to the pause screen
	Main::pauseMenu->addToViewport(true);
}

void Usettings::setupLocs() {
	if (background)
		background->setLoc(vector{ stuff::screenSize.x / 2.f, stuff::screenSize.y / 2.f } - background->getSize() / 2);

	if (backButton) {
		vector backButtonSize = backButton->getSize();
		backButton->setLoc({ float(background->getLoc().x + (background->w - 4) * stuff::pixelSize - backButtonSize.x / 2), float(background->getLoc().y - backButtonSize.y / 2 + 4 * stuff::pixelSize)});
	}

	vector scrollBoxLoc = background->getLoc() + vector{10, 8} *stuff::pixelSize;
	vector scrollBoxSize = vector{ (23 * 14) * stuff::pixelSize, background->getSize().y - 15 * stuff::pixelSize };
	scrollBox->setLocAndSize(scrollBoxLoc, scrollBoxSize);
	scrollBox->setOgLoc(scrollBoxLoc);

	if (cancelButton && background && saveButton)
		cancelButton->setLoc(background->getLoc() + background->getSize() - saveButton->getSize() - vector{10, 10} *stuff::pixelSize);
	if (cancelText && cancelButton)
		cancelText->setLoc(cancelButton->loc + cancelButton->getSize() / 2 - cancelText->getSize() / 2);
	if (saveButton && cancelButton)
		saveButton->setLoc(cancelButton->getLoc() - vector{saveButton->getSize().x + 2 * stuff::pixelSize, 0 });
	if (saveText && saveButton)
		saveText->setLoc(saveButton->loc + saveButton->getSize() / 2 - saveText->getSize() / 2);
}

void Usettings::cancel() {
	// just revert all settings back to original before save

}

void Usettings::setupText() {

}

void Usettings::togglePixelText() {
	std::cout << "changing text" << std::endl;
	SaveData::settingsData.pixelFont = !SaveData::settingsData.pixelFont;
	text::changeFontAll();
}

void Usettings::toggleShortNumbers() {
	SaveData::settingsData.shortNumbers = !SaveData::settingsData.shortNumbers;
	//shortNumbersButtonText->setText(SaveData::settingsData.shortNumbers ? "On" : "Off");
}

void Usettings::togglePets() {
	SaveData::settingsData.showPets = !SaveData::settingsData.showPets;
}

void Usettings::toggleRain() {
	SaveData::settingsData.showRain = !SaveData::settingsData.showRain;
}