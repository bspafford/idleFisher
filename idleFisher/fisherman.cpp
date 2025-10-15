#include "fisherman.h"
#include "fishermanWidget.h"
#include "main.h"

Afisherman::Afisherman(vector loc) : npc(loc) {
	setup("fisherman");
	fishermanWidget = std::make_unique<UfishermanWidget>(this);

	int worldId = math::getWorldIndexFromName(Main::currWorldName);
	discovered = &SaveData::saveData.npcSave[worldId].fishermanDiscovered;
}

Afisherman::~Afisherman() {

}

void Afisherman::click() {
	// set save data discovered to true
	*discovered = true;
	fishermanWidget->addToViewport(true);
}