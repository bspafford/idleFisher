#pragma once

#include "widget.h"

class text;
class UscrollBox;
class Ubutton;
class Uslider;
class horizontalBox;

class Usettings : public widget {
public:
	Usettings();
	~Usettings();
	void draw(Shader* shaderProgram);
	void setupLocs() override;
private:
	void goBack();
	void cancel();

	// setting buttons
	void setupText();
	void togglePixelText();
	void toggleShortNumbers();
	void togglePets();
	void toggleRain();

	Image* background;
	Ubutton* backButton;
	UscrollBox* scrollBox;

	// settings
	text* settingsTitle;
	Ubutton* saveButton;
	text* saveText;
	Ubutton* cancelButton;
	text* cancelText;

	// audio
	text* audioTitle;
	Uslider* masterVolumeSlider;
	Uslider* musicVolume;
	Uslider* sfxVolume;
	Uslider* dialogVolume;


	// graphics
	text* graphicsTitle;
	// pixel font
	horizontalBox* pixelFontHorizBox;
	text* pixelFontText;
	Ubutton* pixelFontButton;
	// short numbers
	horizontalBox* shortNumHorizBox;
	text* shortNumbersText;
	Ubutton* shortNumbersButton;
	// show pets
	horizontalBox* petHorizBox;
	text* petText;
	Ubutton* petButton;
	// show rain
	horizontalBox* rainHorizBox;
	text* rainText;
	Ubutton* rainButton;
};