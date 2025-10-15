#include "decorator.h"
#include "decoratorWidget.h"

Adecorator::Adecorator(vector loc) : npc(loc) {
	setup("decorator");

	decoratorWidget = std::make_unique<UdecoratorWidget>(this);

	discovered = &discoveredFallback;
}

void Adecorator::click() {
	decoratorWidget->addToViewport(true);
}