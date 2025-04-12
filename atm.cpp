#include "atm.h"
#include "currencyConverterWidget.h"
#include "main.h"
#include "timer.h"
#include "currencyWidget.h"
#include "upgrades.h"

Aatm::Aatm(vector loc) : npc(loc) {
	for (int i = 0; i < SaveData::data.currencyData.size(); i++) {
		timer* conversionTimer = new timer();
		conversionTimers.push_back(conversionTimer);
	}

	setup("atm");
	currencyConverterWidget = new UcurrencyConverterWidget(this);

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].atmDiscovered;
}

Aatm::~Aatm() {
	delete currencyConverterWidget;
}

void Aatm::click() {
	*discovered = true;

	currencyConverterWidget->addToViewport(true);
}

void Aatm::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	for (int i = 0; i < conversionTimers.size(); i++) {
		if (conversionTimers[i]->finished())
			finishConversion(i);
	}
}

void Aatm::startConversion(FcurrencyStruct* currency, FsaveCurrencyStruct* saveCurrency, FcurrencyConversionStruct* currencyConversionStruct) {
	currencyConversionStruct->converting = true;
	conversionTimers[currency->id]->start(upgrades::calcConversionTime(currencyConversionStruct));
}

void Aatm::stopConversion(FcurrencyStruct* currency, FsaveCurrencyStruct* saveCurrency, FcurrencyConversionStruct* currencyConversionStruct) {
	currencyConversionStruct->converting = false;
	conversionTimers[currency->id]->stop();
}

void Aatm::finishConversion(int id) {
	FcurrencyConversionStruct* conversionStruct = &SaveData::saveData.currencyConversionList[id];
	if (SaveData::saveData.currencyList[id].numOwned >= conversionStruct->price) {
		SaveData::saveData.currencyList[id].numOwned -= conversionStruct->price;
		SaveData::saveData.currencyList[id + 1].numOwned += conversionStruct->yield; // i don't think you should add to total currency
		Main::currencyWidget->updateList();
	}

	conversionTimers[id]->start(upgrades::calcConversionTime(conversionStruct));
}

void Aatm::calcIdleProfits(float timeDiff) {
	for (int i = 0; i < SaveData::saveData.currencyList.size(); i++) {
		FcurrencyStruct currency = SaveData::data.currencyData[i];
		FsaveCurrencyStruct* saveCurrency = &SaveData::saveData.currencyList[currency.id];
		FcurrencyConversionStruct* conversion = &SaveData::saveData.currencyConversionList[currency.id];
		// continues if premium || last currency
		if (!conversion->converting || currency.id == 0 || currency.id == SaveData::data.currencyData[SaveData::data.currencyData.size()-1].id)
			continue;

		// calcs max conversions based on time
		float maxConversionsTime = floor(timeDiff / upgrades::calcConversionTime(conversion));

		// calc max conversions based on currency
		float maxConversionsCurrency = floor(saveCurrency->numOwned / conversion->price);

		// takes min conversion
		float minConversions = math::min(maxConversionsTime, maxConversionsCurrency);

		saveCurrency->numOwned -= conversion->price * minConversions;
		SaveData::saveData.currencyList[currency.id + 1].numOwned += conversion->yield * minConversions;
	}

	Main::currencyWidget->updateList();
}