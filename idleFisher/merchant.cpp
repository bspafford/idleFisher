#include "merchant.h"
#include "merchantWidget.h"
#include "main.h"

Amerchant::Amerchant(vector loc) : npc(loc) {
	setup("merchant");
	merchantWidget = new UmerchantWidget(this);

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].merchantDiscovered;
}

Amerchant::~Amerchant() {
	delete merchantWidget;
}

void Amerchant::click() {
	*discovered = true;
	merchantWidget->addToViewport(true);
}