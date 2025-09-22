#include "sailor.h"
#include "sailorWidget.h"
#include "main.h"

Asailor::Asailor(vector loc) : npc(loc) {
	setup("sailor");
	sailorWidget = new UsailorWidget();

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].sailorDiscovered;
}

Asailor::~Asailor() {
	delete sailorWidget;
}

void Asailor::click() {
	*discovered = true;

	sailorWidget->addToViewport(true);
}