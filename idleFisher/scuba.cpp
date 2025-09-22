#include "scuba.h"
#include "rebirthWidget.h"
#include "main.h"

Ascuba::Ascuba(vector loc) : npc(loc) {
	setup("scuba");
	//widget->setup(SaveData::data.fishingRodData, SaveData::saveData.fishingRodList);

	rebirthWidget = new UrebirthWidget();

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].scubaDiscovered;
}

void Ascuba::click() {
	*discovered = true;

	rebirthWidget->addToViewport(true);
}