#pragma once

#include <string>
#include <SDL.h>

#include "saveData.h"

class Eshake;

class UfishComboWidget {
public:
	UfishComboWidget(SDL_Renderer* renderer);
	~UfishComboWidget();

	int click(bool fishing);
	void Start(FfishData fish, int quality);
	void Update(float deltaTime);

	float calcYellowSize();
	float calcGreenSize();
	FfishData currFish;
	int quality;

	void updateComboSize();

	void draw(SDL_Renderer* renderer, int screenWidth, int screenHeight);
	SDL_Rect getRect(int screenX, int screenY, int sizeX, int sizeY);
	void setupRandomCombo();
	void updateComboStartEnd();
	int getCombo();

	SDL_Rect greenRect;
	SDL_Rect yellowRect;
	SDL_Rect fishRect;

	int comboStart = 0, comboEnd = 0;
	int comboLoc = 0;

	bool visible = false;

	float fishLoc = 0; // percent of its location on bar

	// both percents of full bar
	float greenSize = 0;
	float yellowSize = 0;
	float fishSpeed = 0;

	// widget components
	std::string fishComboBorderPath = "./images/widget/fishComboBorder.png";
	SDL_Texture* fishComboBorderImg;
	int fishComboBorderX, fishComboBorderY;

	std::string fishPath = "./images/widget/fish.png";
	SDL_Texture* fishImg;
	int fishX, fishY;

	// should move backwards or forwards
	bool fishMoveBack = false;
	int hitWallNum = 0;

	Eshake* shake;
};