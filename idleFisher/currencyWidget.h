#pragma once

#include "widget.h"
#include "fishNumWidget.h"

class UcurrencyWidget : public widget {
public:
	~UcurrencyWidget();

	void updateList();
	void draw(Shader* shaderProgram);

	std::vector<UfishNumWidget*> currencyList;
};
