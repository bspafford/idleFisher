#include "scuba.h"
#include "main.h"

Ascuba::Ascuba(vector loc) : npc(loc) {
	setup("scuba");

	rebirthWidget = std::make_unique<UrebirthWidget>(nullptr);

	int worldId = math::getWorldIndexFromName(Main::currWorldName);
	discovered = &SaveData::saveData.npcSave[worldId].scubaDiscovered;
}

void Ascuba::click() {
	*discovered = true;

	rebirthWidget->addToViewport(true);
}