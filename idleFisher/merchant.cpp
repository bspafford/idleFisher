#include "merchant.h"
#include "merchantWidget.h"
#include "main.h"

Amerchant::Amerchant(vector loc) : npc(loc) {
	setup("merchant");
	merchantWidget = std::make_unique<UmerchantWidget>(this);

	int worldId = math::getWorldIndexFromName(Main::currWorldName);
	discovered = &SaveData::saveData.npcSave[worldId].merchantDiscovered;
}

Amerchant::~Amerchant() {

}

void Amerchant::click() {
	*discovered = true;
	merchantWidget->addToViewport(true);
}