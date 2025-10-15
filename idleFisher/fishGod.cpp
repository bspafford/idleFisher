#include "fishGod.h"
#include "rebirthWidget.h"
#include "main.h"

AfishGod::AfishGod(vector loc) : npc(loc) {
	setup("fishGod");
	
	rebirthWidget = std::make_unique<UrebirthWidget>();

	int worldId = math::getWorldIndexFromName(Main::currWorldName);
	discovered = &discoveredFallback;
}

void AfishGod::click() {
	rebirthWidget->addToViewport(true);
}