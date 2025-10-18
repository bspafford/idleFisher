#pragma once

#include "widget.h"
#include "fishNumWidget.h"

class UcurrencyWidget : public widget {
public:
	UcurrencyWidget(widget* parent);
	~UcurrencyWidget();

	void updateList();
	void draw(Shader* shaderProgram);

	std::vector<std::unique_ptr<UfishNumWidget>> currencyList;
};
