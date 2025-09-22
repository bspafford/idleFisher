#include "fishGod.h"
#include "rebirthWidget.h"
#include "main.h"

AfishGod::AfishGod(vector loc) : npc(loc) {
	setup("fishGod");
	
	rebirthWidget = new UrebirthWidget();

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	discovered = new bool(true);
}

void AfishGod::click() {
	rebirthWidget->addToViewport(true);
}