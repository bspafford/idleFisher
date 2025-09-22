#pragma once

#include "npc.h"

class ApetSeller : public npc {
public:
	ApetSeller(vector loc);
	void click() override;
};