#pragma once

#include "npc.h"

class UfishermanWidget;

class Afisherman : public npc {
public:
	Afisherman(vector loc);
	~Afisherman();

	void click() override;
private:
	std::unique_ptr<UfishermanWidget> fishermanWidget;
};