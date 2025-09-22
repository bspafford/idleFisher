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

			widget->setLoc(vector{ widget->getLoc().x, yOffset }.round());
			widget->setup(currencyStruct, saveCurrency.numOwned);
			yOffset += widget->getSize().y + 1 * stuff::pixelSize;
		}
	}

}

void UcurrencyWidget::draw(Shader* shaderProgram) {
	float yOffset = 15 * stuff::pixelSize;
	for (int i = 0; i < currencyList.size(); i++) {
		currencyList[i]->setLoc(vector{ stuff::screenSize.x - currencyList[i]->getSize().x - 11 * stuff::pixelSize, yOffset}.round());
		currencyList[i]->draw(shaderProgram);
		yOffset += currencyList[i]->getSize().y + 1 * stuff::pixelSize;
	}
}