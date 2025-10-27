#pragma once

#include "widget.h"

class text;
class UscrollBox;
class Ubutton;
class Uslider;
class horizontalBox;
class UsettingsBlock;

class Usettings : public widget {
public:
	Usettings(widget* parent);
	~Usettings();
	void draw(Shader* shaderProgram);
	void setupLocs() override;
private:
	void goBack();
	void cancel();

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
	std::unique_ptr<UsettingsBlock> pixelFontBlock;
	std::unique_ptr<UsettingsBlock> shortNumBlock;
	std::unique_ptr<UsettingsBlock> petBlock;
	std::unique_ptr<UsettingsBlock> rainBlock;
	std::unique_ptr<UsettingsBlock> cursorBlock;
};