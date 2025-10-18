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
	UcurrencyConverterBox(widget* parent, FcurrencyStruct* currencyStruct, FsaveCurrencyStruct* saveCurrencyStruct);
	~UcurrencyConverterBox();
	void setup();
	void update();

	void draw(Shader* shaderProgram) override;

	vector getSize() override;

private:
	void buyUpgrade();

	UcurrencyConverterWidget* widgetParent;

	// structs
	FcurrencyStruct* currencyStruct;
	FsaveCurrencyStruct* saveCurrencyStruct;

	std::unique_ptr<Image> background;

	std::unique_ptr<Image> currency1;
	std::unique_ptr<text> currency1Num;
	std::unique_ptr<Image> currency2;
	std::unique_ptr<text> currency2Num;
	std::unique_ptr<Image> arrow;

	std::string nameString;
	std::string descriptionString;

	std::unique_ptr<text> upgradeText;
	std::unique_ptr<text> buttonPriceText;

public:
	std::unique_ptr<Ubutton> buyButton;
protected:
	void setupLocs() override;
private:
	void setLocAndSize(vector loc, vector size) override;


	bool mouseOver();

	std::function<void()> callback;
};