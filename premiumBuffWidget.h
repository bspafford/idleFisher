#pragma once

#include "widget.h"
#include "saveData.h"

class timer;
class UprogressBar;
class UhoverBox;
struct Fimg;

class UpremiumBuffWidget : public widget {
public:
	UpremiumBuffWidget(FgoldenFishStruct goldenFish);
	void draw(SDL_Renderer* renderer);
	void setLoc(vector loc) override;
	vector getSize() override;
	FgoldenFishStruct getGoldenFish();
private:
	FgoldenFishStruct goldenFish;
	void timerFinished();
	void timerUpdate();
	timer* lifeTimer;

	UprogressBar* progressBar;
	Fimg* img;
	UhoverBox* hoverBox;
};