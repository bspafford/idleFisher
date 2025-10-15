#pragma once

#include "widget.h"
#include "text.h"

class Shader;
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

	std::unique_ptr<Image> thumbnail;

	std::unique_ptr<text> numText;

	int yOffset = 0;

	std::unique_ptr<Image> star;

	FfishData* fish;
	FsaveFishData* saveFish;
	int fishQuality;
	double currencyNum;
};