#pragma once

#include "widget.h"
#include "saveData.h"

class timer;
class UprogressBar;
class UhoverBox;
class Image;

class UpremiumBuffWidget : public widget {
public:
	UpremiumBuffWidget(FgoldenFishStruct goldenFish);
	void draw(Shader* shaderProgram);
	void setLoc(vector loc) override;
	vector getSize() override;
	FgoldenFishStruct getGoldenFish();
private:
	FgoldenFishStruct goldenFish;
	void timerFinished();
	void timerUpdate();
	std::unique_ptr<timer> lifeTimer;

	std::unique_ptr<UprogressBar> progressBar;
	std::unique_ptr<Image> img;
	std::unique_ptr<UhoverBox> hoverBox;
};