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

	void draw(Shader* shaderProgram);
	void setLoc(vector loc) override;
	void updateUI();

	FfishData* fishData;
	FsaveFishData* saveFishData;
	 
private:
	bool hasAllStars();
	bool hasBiggestSize();
	bool isUnlocked();

	void openFishPage();

	class Ujournal* ref;

	//Image* img;
	Ubutton* fishButton;
	//Image* thumbnail;
	text* name;
	Image* checkMark;
	//text* price;
	//text* total; // caught
};