#include "dumpster.h"

#include "main.h"
#include "worlds.h"
#include "saveData.h"
#include "AautoFisher.h"
#include "autoFisherUI.h"
#include "upgrades.h"

// widget
#include "heldFishWidget.h"
#include "currencyWidget.h"

dumpster::dumpster(vector loc) {
	this->loc = loc;

	img = new Fimg("./images/dumpster.png", loc);
	img->loadSurface(img->path);
}

void dumpster::onHover() {

	bool temp = bMouseOver;
	bMouseOver = mouseOver();
	if (!temp && bMouseOver) {
		img->setImage("./images/dumpsterHovered.png", loc);
		Main::hoverObject(NULL);
	} else if (temp && !bMouseOver) {
		img->setImage("./images/dumpster.png", loc);
		Main::leaveHoverObject(NULL);
	}

	if (bMouseOver && Main::bLeftClick && !Main::currWidget)
		Main::addLeftClick(this, &dumpster::sellFish);
}

void dumpster::draw(SDL_Renderer* renderer) {
	onHover();

	img->draw(renderer);
}

void dumpster::sellFish() {
	for (int i = 0; i < SaveData::saveData.fishData.size(); i++) {
		FsaveFishData* currSaveFish = &SaveData::saveData.fishData[i];
		FfishData* currFish = &SaveData::data.fishData[currSaveFish->id];

		for (int j = 0; j < SaveData::saveData.fishData[i].numOwned.size(); j++) {
			double currencyGained = currSaveFish->numOwned[j] * upgrades::getFishSellPrice(*currFish, j);
			if (currencyGained > 0)
				SaveData::saveData.currencyList[currFish->currencyId].unlocked = true;
			SaveData::saveData.currencyList[currFish->currencyId].numOwned += currencyGained;
			SaveData::saveData.currencyList[currFish->currencyId].totalNumOwned += currencyGained;
			currSaveFish->numOwned[j] = 0;
		}
	}

	Main::heldFishWidget->updateList(Main::renderer);
	Main::currencyWidget->updateList();

	// updates UI max
	for (int i = 0; i < world::autoFisherList.size(); i++)
		world::autoFisherList[i]->UI->updateUI();
}

bool dumpster::mouseOver() {
	Main::mousePos;

	vector min = math::worldToScreen(img->loc);
	vector max = min + (vector{ float(img->w), float(img->h) } *stuff::pixelSize);

	if (min.x <= Main::mousePos.x && Main::mousePos.x <= max.x && min.y <= Main::mousePos.y && Main::mousePos.y <= max.y) {
		vector screenPos = min;
		vector pos = { Main::mousePos.x - screenPos.x, Main::mousePos.y - screenPos.y };
		SDL_Color pixelColor = math::GetPixelColor(img->surface, (int)pos.x, (int)pos.y);

		if ((int)pixelColor.a != 0)
			return true;
	}

	return false;
}