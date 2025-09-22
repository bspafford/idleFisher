#pragma once

#include <string>

#include "saveData.h"

class Shader;
class Image;
class URectangle;
class Eshake;

class UfishComboWidget {
public:
	UfishComboWidget();
	~UfishComboWidget();

	int click(bool fishing);
	void Start(FfishData fish, int quality);
	void Update(float deltaTime);

	float calcYellowSize();
	float calcGreenSize();
	FfishData currFish;
	int quality;

	void updateComboSize();

	void draw(Shader* shaderProgram, int screenWidth, int screenHeight);
	//SDL_Rect getRect(int screenX, int screenY, int sizeX, int sizeY);
	void setupRandomCombo();
	void updateComboStartEnd();
	int getCombo();

	float getValidWidth();

	URectangle* greenRect;
	URectangle* yellowRect;
	URectangle* backgroundRect;

	int comboStart = 0, comboEnd = 0;
	int comboLoc = 0;

	bool visible = false;

	float fishLoc = 0; // percent of its location on bar
	float fishSpeed = 0;

	// widget components
	Image* fishComboBorderImg;
	int fishComboBorderX, fishComboBorderY;

	Image* fishImg;
	int fishX, fishY;

	// should move backwards or forwards
	bool fishMoveBack = false;
	int hitWallNum = 0;

	Eshake* shake;
};