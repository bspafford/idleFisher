#pragma once

#include "widget.h"

class text;
class UscrollBox;
class Ubutton;
class Uslider;
class horizontalBox;

class Usettings : public widget {
public:
	Usettings(widget* parent);
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

	std::unique_ptr<Image> background;
	std::unique_ptr<Ubutton> backButton;
	std::unique_ptr<UscrollBox> scrollBox;

	// settings
	std::unique_ptr<text> settingsTitle;
	std::unique_ptr<Ubutton> saveButton;
	std::unique_ptr<text> saveText;
	std::unique_ptr<Ubutton> cancelButton;
	std::unique_ptr<text> cancelText;

	// audio
	std::unique_ptr<text> audioTitle;
	std::unique_ptr<Uslider> masterVolumeSlider;
	std::unique_ptr<Uslider> musicVolume;
	std::unique_ptr<Uslider> sfxVolume;
	std::unique_ptr<Uslider> dialogVolume;


	// graphics
	std::unique_ptr<text> graphicsTitle;
	// pixel font
	std::unique_ptr<horizontalBox> pixelFontHorizBox;
	std::unique_ptr<text> pixelFontText;
	std::unique_ptr<Ubutton> pixelFontButton;
	// short numbers
	std::unique_ptr<horizontalBox> shortNumHorizBox;
	std::unique_ptr<text> shortNumbersText;
	std::unique_ptr<Ubutton> shortNumbersButton;
	// show pets
	std::unique_ptr<horizontalBox> petHorizBox;
	std::unique_ptr<text> petText;
	std::unique_ptr<Ubutton> petButton;
	// show rain
	std::unique_ptr<horizontalBox> rainHorizBox;
	std::unique_ptr<text> rainText;
	std::unique_ptr<Ubutton> rainButton;
};