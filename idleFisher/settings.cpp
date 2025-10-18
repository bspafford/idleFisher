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

Usettings::Usettings(widget* parent) : widget(parent) {
	SaveData::loadSettings();

	// button click anim
	std::vector<std::string> buttonClickAnim;
	for (int i = 0; i < 2; i++)
		buttonClickAnim.push_back("./images/widget/upgradeButton" + std::to_string(i + 1) + ".png");

	background = std::make_unique<Image>("./images/widget/achievementBackground.png", vector{ 0, 0 }, false);

	backButton = std::make_unique<Ubutton>(this, "widget/npcXButton.png", 11, 11, 1, vector{ 0, 0 }, false, false);
	backButton->addCallback(this, &Usettings::goBack);

	scrollBox = std::make_unique<UscrollBox>(this);

	settingsTitle = std::make_unique<text>(this, "Settings", "biggerStraight", vector{ 0, 0 });
	audioTitle = std::make_unique<text>(this, "  Audio:", "biggerStraight", vector{ 0, 0 });
	graphicsTitle = std::make_unique<text>(this, "  Graphics:", "biggerStraight", vector{ 0, 0 });

	// padding
	scrollBox->addChild(NULL, 3 * stuff::pixelSize);
// settings
	scrollBox->addChild(settingsTitle.get(), settingsTitle->getSize().y + 3 * stuff::pixelSize);

	saveButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	saveButton->addCallback(SaveData::saveSettings);
	saveText = std::make_unique<text>(this, "Save", "straightDark", vector{ 0, 0 });
	
	cancelButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	cancelButton->addCallback(this, &Usettings::cancel);
	
	cancelText = std::make_unique<text>(this, "Cancel", "straightDark", vector{ 0, 0 });

// audio
	scrollBox->addChild(audioTitle.get(), audioTitle->getSize().y + 3 * stuff::pixelSize);

	masterVolumeSlider = std::make_unique<Uslider>(this, false, vector{ background->w - 50.f, 3}, 0, 100);
	masterVolumeSlider->setSliderTitle("     Master ");
	masterVolumeSlider->bindValue(&SaveData::settingsData.masterVolume);
	musicVolume = std::make_unique<Uslider>(this, false, vector{ background->w - 50.f, 3 }, 0, 100);
	musicVolume->setSliderTitle("     Music   ");
	musicVolume->bindValue(&SaveData::settingsData.musicVolume);
	sfxVolume = std::make_unique<Uslider>(this, false, vector{ background->w - 50.f, 3 }, 0, 100);
	sfxVolume->setSliderTitle("     SFX       ");
	sfxVolume->bindValue(&SaveData::settingsData.sfxVolume);
	dialogVolume = std::make_unique<Uslider>(this, false, vector{ background->w - 50.f, 3 }, 0, 100);
	dialogVolume->setSliderTitle("     Dialog ");
	dialogVolume->bindValue(&SaveData::settingsData.dialogVolume);

	scrollBox->addChild(masterVolumeSlider.get(), masterVolumeSlider->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(musicVolume.get(), musicVolume->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(sfxVolume.get(), sfxVolume->getSize().y + 5 * stuff::pixelSize);
	scrollBox->addChild(dialogVolume.get(), dialogVolume->getSize().y + 5 * stuff::pixelSize);

// graphics
	scrollBox->addChild(graphicsTitle.get(), graphicsTitle->getSize().y + 3 * stuff::pixelSize);

	pixelFontHorizBox;
	pixelFontHorizBox = std::make_unique<horizontalBox>(this);
	std::unique_ptr<text> pixelFontText = std::make_unique<text>(this, "     Pixel Font", "straight", vector{ 0, 0 });

	std::unique_ptr<Ubutton> pixelFontButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	//pixelFontButton->setClickAnim(buttonClickAnim);
	pixelFontButton->addCallback(this, &Usettings::togglePixelText);
	pixelFontHorizBox->addChild(pixelFontText.get(), pixelFontText->getSize().x);
	pixelFontHorizBox->addChild(NULL, 229 * stuff::pixelSize);
	pixelFontHorizBox->addChild(pixelFontButton.get(), pixelFontButton->getSize().x);
	scrollBox->addChild(pixelFontHorizBox.get(), 16 * stuff::pixelSize);

	shortNumHorizBox = std::make_unique<horizontalBox>(this);
	shortNumbersText = std::make_unique<text>(this, "     Enable Short Numbers", "straight", vector{ 0, 0 });
	shortNumbersButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	//shortNumbersButton->setClickAnim(buttonClickAnim);
	shortNumbersButton->addCallback(this, &Usettings::toggleShortNumbers);
	shortNumHorizBox->addChild(shortNumbersText.get(), shortNumbersText->getSize().x);
	shortNumHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	shortNumHorizBox->addChild(shortNumbersButton.get(), shortNumbersButton->getSize().x);
	scrollBox->addChild(shortNumHorizBox.get(), 16 * stuff::pixelSize);

	// show pets
	petHorizBox = std::make_unique<horizontalBox>(this);
	petText = std::make_unique<text>(this, "     Show Pets                     ", "straight", vector{ 0, 0 });
	petButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	//petButton->setClickAnim(buttonClickAnim);
	petButton->addCallback(this, &Usettings::togglePets);
	petHorizBox->addChild(petText.get(), petText->getSize().x);
	petHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	petHorizBox->addChild(petButton.get(), petButton->getSize().x);
	scrollBox->addChild(petHorizBox.get(), 16 * stuff::pixelSize);

	// show rain
	rainHorizBox = std::make_unique<horizontalBox>(this);
	rainText = std::make_unique<text>(this, "     Show Rain                     ", "straight", vector{ 0, 0 });
	rainButton = std::make_unique<Ubutton>(this, "widget/upgradeButton.png", 35, 13, 1, vector{ 0, 0 }, false, false);
	//rainButton->setClickAnim(buttonClickAnim);
	rainButton->addCallback(this, &Usettings::toggleRain);
	rainHorizBox->addChild(rainText.get(), rainText->getSize().x);
	rainHorizBox->addChild(NULL, 191 * stuff::pixelSize);
	rainHorizBox->addChild(rainButton.get(), rainButton->getSize().x);
	scrollBox->addChild(rainHorizBox.get(), 16 * stuff::pixelSize);


	setupText();

	setupLocs();
}

Usettings::~Usettings() {

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