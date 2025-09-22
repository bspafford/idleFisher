#include "mechanic.h"
#include "main.h"
#include "mechanicWidget.h"

Amechanic::Amechanic(vector loc) : npc(loc) {
	setup("mechanic");
	mechanicWidget = new UmechanicWidget(this);

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = &SaveData::saveData.npcSave[worldId].mechanicDiscovered;
}

Amechanic::~Amechanic() {
	//delete mechanicWidget;
}

void Amechanic::click() {
	*discovered = true;
	mechanicWidget->addToViewport(true);
}