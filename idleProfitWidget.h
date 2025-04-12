#pragma once

#include "widget.h"
#include "saveData.h"

class Ubutton;
class text;
class UheldFishWidget;
class UscrollBox;

class UidleProfitWidget : public widget {
public:
	UidleProfitWidget();
	~UidleProfitWidget();
	void draw(SDL_Renderer* renderer) override;
	void setup(std::vector<FsaveFishData> fishList);
private:
	void addedToViewport() override;
	void setupLocs() override;

	// components
	SDL_Rect background;
	Ubutton* collectButton;
	text* title;
	text* collectText;

	UscrollBox* scrollBox;
	UheldFishWidget* heldFishWidget;
};