#include "vaultPlacedItems.h"
#include "main.h"
#include "saveData.h"
#include "button.h"

AvaultPlacedItems::AvaultPlacedItems() {
	// load all the vault stuff
	// place items / make struct
	for (int i = 0; i < SaveData::saveData.vaultUnlockList.size(); i++) {
		FvaultUnlocksStruct* vaultUnlocks = &SaveData::data.vaultUnlockData[i];
		FsaveVaultUnlocksStruct* saveVaultUnlocks = &SaveData::saveData.vaultUnlockList[i];
		if (saveVaultUnlocks->unlocked) {
			// add to list and set loc
			FvaultStuff vaultStuff;

			//vaultStuff.button = new Ubutton(NULL, vaultUnlocks->thumbnailPath, -1, -1, 1, saveVaultUnlocks->loc, "", true);
			vaultStuff.vaultUnlocks = vaultUnlocks;
			vaultStuff.saveVaultUnlocks = saveVaultUnlocks;

			itemList.push_back(vaultStuff);
		}
	}
}

void AvaultPlacedItems::draw(SDL_Renderer* renderer) {
	for (const FvaultStuff vaultStuff : itemList) {
		if (inPlacingMode && selectedItem && selectedItem->vaultUnlocks->id == vaultStuff.vaultUnlocks->id) {
			vector temp = vaultStuff.offset;
			std::cout << "offset1: " << temp << std::endl;
			vaultStuff.saveVaultUnlocks->loc = selectedItem->offset + math::screenToWorld(Main::mousePos);
			vaultStuff.button->setLoc(vaultStuff.saveVaultUnlocks->loc);
		}
		//if (vaultStuff.saveVaultUnlocks->placed || (selectedItem && selectedItem->vaultUnlocks->id == vaultStuff.vaultUnlocks->id))
		//	vaultStuff.button->draw(renderer);
	}

	sortDraw(renderer);

	if (inPlacingMode && Main::bRightClick && !selectedItem)
		putItemInStorage();
	if (inPlacingMode && Main::bLeftClick) {
		if (!selectedItem)
			selectItem();
		else {
			selectedItem->saveVaultUnlocks->placed = true;
			selectedItem = NULL;
		}
	}
}

void AvaultPlacedItems::sortDraw(SDL_Renderer* renderer) {
	// loop through itemList
	// if first them place in list
	// then loop through the list again
	// if base of item is less than the other base
		// put item there in list
	// else
		// move to the next item
	// sort list
	sortedItemList.clear();
	for (int i = 0; i < itemList.size(); i++) {
		if (sortedItemList.size() == 0) {
			sortedItemList.push_back(itemList[i]);
		} else {
			bool added = false;
			for (int j = 0; j < sortedItemList.size(); j++) {
				if (itemList[i].button->getLoc().y + itemList[i].button->getSize().y < sortedItemList[j].button->getLoc().y + sortedItemList[j].button->getSize().y) {
					sortedItemList.insert(sortedItemList.begin() + j, itemList[i]);
					added = true;
					break;
				}
			}
			if (!added)
				sortedItemList.push_back(itemList[i]);
		}
	}

	// draw
	for (FvaultStuff item : sortedItemList)
		item.button->draw(renderer);
}

void AvaultPlacedItems::placeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock) {
	//FvaultStuff vaultStuff;
	/*
	selectedItem = new FvaultStuff();
	selectedItem->button = new Ubutton(NULL, vaultUnlock->thumbnailPath, { 0, 0 }, "", true);
	selectedItem->vaultUnlocks = vaultUnlock;
	selectedItem->saveVaultUnlocks = saveVaultUnlock;
	*/

	if (selectedItem)
		itemList.push_back(*selectedItem);
}

void AvaultPlacedItems::removeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock) {
	for (int i = 0; i < itemList.size(); i++) {
		FvaultStuff vaultStuff = itemList[i];
		if (vaultStuff.vaultUnlocks->id == vaultUnlock->id) {
			saveVaultUnlock->placed = false;
			itemList.erase(itemList.begin() + i);
			break;
		}
	}
}

void AvaultPlacedItems::putItemInStorage() {
	std::cout << "putting item in storage" << std::endl;
	FvaultStuff lastItem;
	for (const FvaultStuff vaultStuff : sortedItemList) {
		if (vaultStuff.button->isMouseOver()) {
			std::cout << "thats the one" << std::endl;
			lastItem = vaultStuff;
		}
	}

	for (int i = 0; i < itemList.size(); i++) {
		if (lastItem.vaultUnlocks->id == itemList[i].vaultUnlocks->id) {
			itemList[i].saveVaultUnlocks->placed = false;
			itemList.erase(itemList.begin() + i);
			break;
		}
	}
}

void AvaultPlacedItems::selectItem() {
	for (const FvaultStuff vaultStuff : sortedItemList) {
		if (vaultStuff.button->isMouseOver()) {
			// don't return to get last item in the last, therefore grabbing item infront
			selectedItem = new FvaultStuff(vaultStuff);
			selectedItem->offset = vaultStuff.button->getLoc() - math::screenToWorld(Main::mousePos);
		}
	}
}