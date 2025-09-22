#pragma once

#include "widget.h"

class Shader;
class text;
struct FfishData;
struct FsaveFishData;
struct FcurrencyStruct;

class UfishNumWidget : public widget {
public:
	~UfishNumWidget();

	// one for fish, and the other for currency
	void setup(FfishData* fish, FsaveFishData* saveFish, int fishQuality);
	void setup(FcurrencyStruct* currency, double num);

	void setupText();

	void setLoc(vector loc) override;
	void draw(Shader* shaderProgram) override; // , float x, float y1

	Image* thumbnail;

	text* numText;

	int yOffset = 0;

	Image* star;

	FfishData* fish;
	FsaveFishData* saveFish;
	int fishQuality;
	double currencyNum;
};