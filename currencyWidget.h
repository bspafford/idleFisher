#pragma once

#include "fishNumWidget.h"

class UcurrencyWidget {
public:
	~UcurrencyWidget();

	void updateList();
	void draw(SDL_Renderer* renderer);

	std::vector<UfishNumWidget*> currencyList;
};
