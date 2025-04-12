#include "currencyWidget.h"

#include "main.h"
#include "saveData.h"

UcurrencyWidget::~UcurrencyWidget() {
	for (int i = 0; i < currencyList.size(); i++)
		delete currencyList[i];
	currencyList.clear();
}

void UcurrencyWidget::updateList() {
	float yOffset = 15 * stuff::pixelSize;

	// setup currency
	currencyList.clear();
	for (int i = 0; i < SaveData::saveData.currencyList.size(); i++) {
		FsaveCurrencyStruct saveCurrency = SaveData::saveData.currencyList[i];

		if (saveCurrency.numOwned != 0) {
			yOffset += stuff::pixelSize;

			UfishNumWidget* widget = new UfishNumWidget();
			currencyList.push_back(widget);

			FcurrencyStruct* currencyStruct = &SaveData::data.currencyData[saveCurrency.id];

			widget->y = yOffset;
			widget->setup(Main::renderer, currencyStruct, saveCurrency.numOwned);
			yOffset += (widget->h + 1) * stuff::pixelSize;
		}
	}

}

void UcurrencyWidget::draw(SDL_Renderer* renderer) {
	float yOffset = 15 * stuff::pixelSize;
	for (int i = 0; i < currencyList.size(); i++) {
		currencyList[i]->draw(renderer, Main::screenWidth - currencyList[i]->widgetSize.x - 11 * stuff::pixelSize, yOffset);
		yOffset += (currencyList[i]->h + 1) * stuff::pixelSize;
	}
}