#include "petSeller.h"
#include "main.h"

ApetSeller::ApetSeller(vector loc) : npc(loc) {
	setup("petSeller");
	widget->setup(SaveData::data.petData, SaveData::saveData.petList);

	int worldId = math::getWorldIndexFromName(Main::currWorldName);
	discovered = &SaveData::saveData.npcSave[worldId].petSellerDiscovered;
}

void ApetSeller::click() {
	*discovered = true;

	__super::click();
}