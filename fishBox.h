#pragma once

#include "widget.h"

class text;
class Ubutton;
struct FfishData;
struct FsaveFishData;

class UfishBox : public widget {
public:
	UfishBox(class Ujournal* ref, FfishData* fishData, FsaveFishData* saveFishData);
	~UfishBox();

	void draw(SDL_Renderer* renderer);
	void setLoc(vector loc) override;
	void updateUI();

	FfishData* fishData;
	FsaveFishData* saveFishData;
	 
private:
	void openFishPage();

	class Ujournal* ref;

	//Fimg* img;
	Ubutton* fishButton;
	//Fimg* thumbnail;
	text* name;
	Fimg* checkMark;
	//text* price;
	//text* total; // caught
};