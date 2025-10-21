#include "petSeller.h"
#include "main.h"

ApetSeller::ApetSeller(vector loc) : npc(loc) {
	setup("petSeller");
	widget->setup(SaveData::data.petData, SaveData::saveData.petList);

	int worldId = Scene::getWorldIndexFromName(Scene::getCurrWorldName());
	discovered = &SaveData::saveData.npcSave[worldId].petSellerDiscovered;
}

void ApetSeller::click() {
	*discovered = true;

	__super::click();
}