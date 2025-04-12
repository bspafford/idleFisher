#pragma once

#include "npc.h"

class UmechanicWidget;

class Amechanic : public npc {
public:
	Amechanic(vector loc);
	~Amechanic();

	void click() override;
private:
	UmechanicWidget* mechanicWidget;
};