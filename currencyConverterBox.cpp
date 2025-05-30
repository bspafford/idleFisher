#include "currencyConverterBox.h"

#include "main.h"
#include "Acharacter.h"
#include "saveData.h"
#include "upgrades.h"
#include "shortNumbers.h"
#include "animation.h"
#include "text.h"
#include "button.h"
#include "verticalBox.h"
#include "wrapBox.h"
#include "currencyConverterWidget.h"
#include "atm.h"

UcurrencyConverterBox::UcurrencyConverterBox(UcurrencyConverterWidget* parent, FcurrencyStruct* currencyStruct, FsaveCurrencyStruct* saveCurrencyStruct) {
	this->widgetParent = parent;

	this->currencyStruct = currencyStruct;
	this->saveCurrencyStruct = saveCurrencyStruct;

	nameString = currencyStruct->name;
	descriptionString = SaveData::data.currencyData[currencyStruct->id + 1].name;

	setup();
}

UcurrencyConverterBox::~UcurrencyConverterBox() {
	delete currency1;
	delete currency2;
	delete buyButton;
	delete buttonPriceText;
	delete upgradeText;
	currency1 = nullptr;
	currency2 = nullptr;
	buyButton = nullptr;
	buttonPriceText = nullptr;
	upgradeText = nullptr;
}

void UcurrencyConverterBox::setup() {
	background = new Fimg("./images/widget/upgradeBoxBackground.png");

	//thumbnailBackground = new Fimg("./images/widget/thumbnailBackground.png");
	//name = new text(nameString, "straight", { 0, 0 });
	currency1 = new Fimg(currencyStruct->thumbnail);
	currency1Num = new text(" ", "straight", {0, 0}, false, false, textAlign::center);
	currency2 = new Fimg(SaveData::data.currencyData[currencyStruct->id+1].thumbnail);
	currency2Num = new text(" ", "straight", {0, 0}, false, false, textAlign::center);
	arrow = new Fimg("./images/widget/arrow.png");

	buyButton = new Ubutton(widgetParent, "./images/widget/upgradeButton.png", 35, 13, 2, { 0, 0 }, false, false);
	//buyButton->setClickAnim({ "./images/widget/upgradeButton1.png", "./images/widget/upgradeButton2.png" });
	buyButton->addCallback(this, &UcurrencyConverterBox::buyUpgrade);

	buttonPriceText = new text("Start", "straightDark", {0, 0}, false, false, textAlign::center);
	buttonPriceText->setTextColor(255, 0, 0);

	// if going
	FcurrencyConversionStruct* currencyConversionStruct = &SaveData::saveData.currencyConversionList[currencyStruct->id];
	if (currencyConversionStruct->converting) {
		Aatm* atm = dynamic_cast<Aatm*>(widgetParent->parent);
		if (atm) {
			atm->startConversion(currencyStruct, saveCurrencyStruct, currencyConversionStruct);
			buttonPriceText->setText("Stop");
			setupLocs(); // updates the locks of the button and the upgrade text
		}
	}

	//update();

	update();
}

void UcurrencyConverterBox::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	background->draw(renderer, false);

	//thumbnailBackground->draw(renderer, false);
	currency1->draw(renderer, false);
	currency1Num->draw(renderer);
	currency2->draw(renderer, false);
	currency2Num->draw(renderer);
	arrow->draw(renderer, false);
	buyButton->draw(renderer);

	buttonPriceText->draw(renderer);

	if (upgradeText)
		upgradeText->draw(renderer);
	// changes the text only if it isn't already set to current world
	if (mouseOver()) {
		if (widgetParent && widgetParent->name->getString() != nameString)
			widgetParent->setNameDescription(nameString, descriptionString);
	}
}

void UcurrencyConverterBox::setLocAndSize(vector loc, vector size) {
	__super::setLocAndSize(loc, size);
	setupLocs();
}

void UcurrencyConverterBox::setupLocs() {
	background->loc = loc;

	vector buttonSize = buyButton->getSize();
	vector buyButtonLoc = loc + vector{ size.x - buttonSize.x, size.y / 2 - buttonSize.y / 2 } + vector{ -10 * stuff::pixelSize, 0 };
	buyButton->setLoc(buyButtonLoc);

	buttonPriceText->setLoc(buyButtonLoc + buyButton->getSize() / 2 - vector{ 0, buttonPriceText->getSize().y / 2 });

	currency1->loc = loc + vector{ 6 * stuff::pixelSize, size.y / 2 - currency1->getSize().y / 2 };
	currency1Num->loc = currency1->loc + vector{ currency1->getSize().x / 2, currency1->getSize().y};
	currency2->loc = currency1->loc + vector{ currency1->getSize().x + 10 * stuff::pixelSize, 0 };
	currency2Num->loc = currency2->loc + vector{ currency2->getSize().x / 2, currency2->getSize().y };
	arrow->loc = currency1->loc + vector{ currency1->getSize().x + stuff::pixelSize, currency1->getSize().y / 2 - arrow->getSize().y / 2 };

	if (upgradeText) {
		vector upgradeTextLoc = { buyButtonLoc.x - stuff::pixelSize, loc.y + size.y / 2 - upgradeText->size.y / 2 };
		upgradeText->setLoc(upgradeTextLoc);
	}
}

bool UcurrencyConverterBox::mouseOver() {
	vector mousePos = Main::mousePos;

	if (!parent && mousePos.x >= loc.x && mousePos.x <= loc.x + size.x && mousePos.y >= loc.y && mousePos.y <= loc.y + size.y)
		return true;
	else if (parent) {
		vector parentLoc = parent->getLoc();
		vector parentSize = parent->getSize();
		if (mousePos.x >= loc.x + parentLoc.x && mousePos.x <= loc.x + size.x + parentLoc.x && mousePos.y >= loc.y + parentLoc.y && mousePos.y <= loc.y + size.y + parentLoc.y)
			// test if inside the rect too
			if (mousePos.x >= parentLoc.x && mousePos.x <= parentLoc.x + parentSize.x && mousePos.y >= parentLoc.y && mousePos.y <= parentLoc.y + parentSize.y)
				return true;
	}
	return false;
}

void UcurrencyConverterBox::buyUpgrade() {
	Aatm* atm = dynamic_cast<Aatm*>(widgetParent->parent);
	if (!atm)
		return;

	FcurrencyConversionStruct* currencyConversionStruct = &SaveData::saveData.currencyConversionList[currencyStruct->id];
	if (currencyConversionStruct->converting) {
		atm->stopConversion(currencyStruct, saveCurrencyStruct, currencyConversionStruct);
		buttonPriceText->setText("Start");
		// start stuff in atm npc
	} else {
		atm->startConversion(currencyStruct, saveCurrencyStruct, currencyConversionStruct);
		buttonPriceText->setText("Stop");
	}

	setupLocs(); // updates the locks of the button and the upgrade text
}

void UcurrencyConverterBox::update() {
	// if going
	/*
	if () { // if max
		buyButton->setImg("./images/widget/upgradeButton3.png");
		buttonPriceText->setText("Stop");
	} */

	FcurrencyConversionStruct conversion = SaveData::saveData.currencyConversionList[currencyStruct->id];
	currency1Num->setText(shortNumbers::convert2Short(conversion.price));
	currency2Num->setText(shortNumbers::convert2Short(conversion.yield));
	setupLocs();
}

vector UcurrencyConverterBox::getSize() {
	return vector{ float(background->w), float(background->h) } *stuff::pixelSize;
}