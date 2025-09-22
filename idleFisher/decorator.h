#pragma once

#include "npc.h"

class UdecoratorWidget;

class Adecorator : public npc {
public:
	Adecorator(vector loc);

private:
	void click() override;

	UdecoratorWidget* decoratorWidget;
};