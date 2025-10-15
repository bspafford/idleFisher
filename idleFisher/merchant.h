#pragma once

#include "npc.h"

class UmerchantWidget;

class Amerchant : public npc {
public:
	Amerchant(vector loc);
	~Amerchant() override;

	void click() override;
private:
	std::unique_ptr<UmerchantWidget> merchantWidget;
};