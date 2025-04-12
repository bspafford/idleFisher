#include "decorator.h"
#include "decoratorWidget.h"

Adecorator::Adecorator(vector loc) : npc(loc) {
	setup("decorator");
	//widget->setup(SaveData::data.vaultUnlockData, SaveData::saveData.vaultUnlockList);

	decoratorWidget = new UdecoratorWidget(this);

	discovered = new bool(true);
}

void Adecorator::click() {
	decoratorWidget->addToViewport(true);
}