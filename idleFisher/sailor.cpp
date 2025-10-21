#include "sailor.h"
#include "sailorWidget.h"
#include "main.h"

Asailor::Asailor(vector loc) : npc(loc) {
	setup("sailor");
	sailorWidget = std::make_unique<UsailorWidget>(nullptr);

	int worldId = Scene::getWorldIndexFromName(Scene::getCurrWorldName());
	discovered = &SaveData::saveData.npcSave[worldId].sailorDiscovered;
}

Asailor::~Asailor() {

}

void Asailor::click() {
	*discovered = true;

	sailorWidget->addToViewport(true);
}