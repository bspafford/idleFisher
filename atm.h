#pragma once

#include "npc.h"

class UcurrencyConverterWidget;
class timer;

class Aatm : public npc {
public:
	Aatm(vector loc);
	~Aatm();

	void draw(SDL_Renderer* renderer) override;

	void click() override;

	void startConversion(FcurrencyStruct* currency, FsaveCurrencyStruct* saveCurrency, FcurrencyConversionStruct* currencyConversionStruct);
	void stopConversion(FcurrencyStruct* currency, FsaveCurrencyStruct* saveCurrency, FcurrencyConversionStruct* currencyConversionStruct);
	void finishConversion(int id);
	void calcIdleProfits(float timeDiff);
private:
	UcurrencyConverterWidget* currencyConverterWidget;

	std::vector<timer*> conversionTimers;
};