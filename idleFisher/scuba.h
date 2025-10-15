#pragma once

#include "npc.h"
#include "rebirthWidget.h"

class Ascuba : public npc {
public:
	Ascuba(vector loc);
private:
	void click() override;

	std::unique_ptr<UrebirthWidget> rebirthWidget;
};