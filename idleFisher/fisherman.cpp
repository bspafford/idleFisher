#include "fisherman.h"
#include "fishermanWidget.h"
#include "main.h"

#include "debugger.h"

Afisherman::Afisherman(vector loc) : npc(loc) {
	setup("fisherman");
	fishermanWidget = std::make_unique<UfishermanWidget>(nullptr, this);

	int worldId = Scene::getWorldIndexFromName(Scene::getCurrWorldName());
	discovered = &SaveData::saveData.npcSave[worldId].fishermanDiscovered;
}

Afisherman::~Afisherman() {

}

void Afisherman::click() {
	// set save data discovered to true
	*discovered = true;
	fishermanWidget->addToViewport(true);
}