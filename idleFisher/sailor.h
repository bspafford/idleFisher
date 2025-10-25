#pragma once

#include "npc.h"
#include "sailorWidget.h"

class Asailor : public npc {
public:
	Asailor(vector loc);
	~Asailor() override;

	void click() override;

private:
	std::unique_ptr<UsailorWidget> sailorWidget;
};
