#pragma once

#include "npc.h"

class UrebirthWidget;

class Ascuba : public npc {
public:
	Ascuba(vector loc);
private:
	void click() override;

	UrebirthWidget* rebirthWidget;
};