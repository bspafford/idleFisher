#pragma once

#include "npc.h"

class UrebirthWidget;

class AfishGod : public npc {
public:
	AfishGod(vector loc);
private:
	void click() override;

	std::unique_ptr<UrebirthWidget> rebirthWidget;
};