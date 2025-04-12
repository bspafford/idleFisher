#include "fisherman.h"
#include "fishermanWidget.h"
#include "main.h"

Afisherman::Afisherman(vector loc) : npc(loc) {
	setup("fisherman");
	fishermanWidget = new UfishermanWidget(this);
	//widget->setup(SaveData::data.fishingRodData, SaveData::saveData.fishingRodList);

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].fishermanDiscovered;
}

Afisherman::~Afisherman() {
	delete fishermanWidget;
}

void Afisherman::click() {
	// set save data discovered to true
	*discovered = true;
	fishermanWidget->addToViewport(true);
	//__super::click();
}