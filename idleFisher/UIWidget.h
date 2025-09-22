#pragma once

#include "widget.h"

class text;
class UpremiumBuffWidget;
class UprogressBar;
class Ubutton;
class timer;

class UUIWidget : public widget {
public:
	UUIWidget();
	~UUIWidget();
	void draw(Shader* shaderProgram) override;
	void updateProgressBar(double points, double progress, double needed);
	void setupLocs() override;

private:
	// value between 0-1

	UprogressBar* progressBar;
	//SDL_Rect* progressBackground;
	//SDL_Rect* progressBar;
	text* rebirthPointNum;
	text* currRunRebirthPoints; // how many rebirth points you have for that run

	Ubutton* NPCshowButton;

	Ubutton* merchantButton;
	Ubutton* fishermanButton;
	Ubutton* mechanicButton;
	Ubutton* petSellerButton;
	Ubutton* sailorButton;
	Ubutton* atmButton;

	void showNPCButtons();

	// show npc button stuff
	timer* buttonsTimer;
	void updateButtonsLoc();
	void finishButtons();
	std::vector<Ubutton*> buttonsList;
	bool open = false;
	bool canOpenClose = true;

	void openMerchantWidget();
	void openFishermanWidget();
	void openMechanicWidget();
	void openPetSellerWidget();
	void openSailorWidget();
	void openAtmWidget();
};