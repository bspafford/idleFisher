#pragma once

#include "widget.h"

#include <functional>

class text;
class Ubutton;
class verticalBox;
class UcurrencyConverterWidget;
struct FcurrencyStruct;
struct FsaveCurrencyStruct;

class UcurrencyConverterBox : public widget {
public:
	UcurrencyConverterBox(UcurrencyConverterWidget* parent, FcurrencyStruct* currencyStruct, FsaveCurrencyStruct* saveCurrencyStruct);
	~UcurrencyConverterBox();
	void setup();
	void update();

	void draw(Shader* shaderProgram) override;

	vector getSize() override;

private:
	void buyUpgrade();

	UcurrencyConverterWidget* widgetParent;
	widget* wrapBox;

	// structs
	FcurrencyStruct* currencyStruct;
	FsaveCurrencyStruct* saveCurrencyStruct;

	Image* background;

	Image* currency1;
	text* currency1Num;
	Image* currency2;
	text* currency2Num;
	Image* arrow;

	std::string nameString;
	std::string descriptionString;

	text* upgradeText;
	text* buttonPriceText;

public:
	Ubutton* buyButton;
protected:
	void setupLocs() override;
private:
	void setLocAndSize(vector loc, vector size) override;


	bool mouseOver();

	std::function<void()> callback;
};