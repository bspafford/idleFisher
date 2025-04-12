#include "upgradeBox.h"

#include "NPCwidget.h"
#include "main.h"
#include "Acharacter.h"
#include "saveData.h"
#include "pet.h"
#include "upgrades.h"
#include "shortNumbers.h"
#include "animation.h"
#include "text.h"
#include "button.h"
#include "verticalBox.h"
#include "wrapBox.h"
#include "merchantWidget.h"
#include "fishermanWidget.h"

UupgradeBox::UupgradeBox(widget* parent, FworldStruct* worldStruct, FsaveWorldStruct* saveWorldStruct) {
	this->widgetParent = parent;

	this->worldStruct = worldStruct;
	this->saveWorldStruct = saveWorldStruct;

	nameString = worldStruct->name;
	descriptionString = worldStruct->description;

	unlocked = &saveWorldStruct->unlocked;
	price = &worldStruct->currencyNum;
	currencyId = &worldStruct->currencyId;

	callback = std::bind(&UupgradeBox::openWorld, this);

	setup();
}

UupgradeBox::UupgradeBox(widget* parent, FfishingRodStruct* fishingRodStruct, FsaveFishingRodStruct* saveFishingRodStruct) {
	this->widgetParent = parent;

	this->fishingRodStruct = fishingRodStruct;
	this->saveFishingRodStruct = saveFishingRodStruct;

	nameString = fishingRodStruct->name;
	descriptionString = fishingRodStruct->description;

	unlocked = &saveFishingRodStruct->unlocked;
	price = &fishingRodStruct->currencyNum;
	currencyId = &fishingRodStruct->currencyId;

	callback = std::bind(&UupgradeBox::equipFishingRod, this);

	thumbnail = new Fimg(fishingRodStruct->imgPath);

	setup();
}

UupgradeBox::UupgradeBox(widget* parent, FbaitStruct* baitStruct, FsaveBaitStruct* saveBaitStruct) {
	this->widgetParent = parent;

	this->baitStruct = baitStruct;
	this->saveBaitStruct = saveBaitStruct;

	nameString = baitStruct->name;
	descriptionString = baitStruct->description;
	buffString = baitStruct->buffs;
	debuffString = baitStruct->debuffs;

	unlocked = &saveBaitStruct->unlocked;
	price = &baitStruct->currencyNum;
	currencyId = &baitStruct->currencyId;

	callback = std::bind(&UupgradeBox::equipBait, this);

	thumbnail = new Fimg(baitStruct->thumbnail);

	setup();
}

UupgradeBox::UupgradeBox(widget* parent, FpetStruct* petStruct, FsavePetStruct* savePetStruct) {
	this->widgetParent = parent;

	this->petStruct = petStruct;
	this->savePetStruct = savePetStruct;

	nameString = petStruct->name;
	descriptionString = petStruct->description;

	unlocked = &savePetStruct->unlocked;
	price = &petStruct->currencyNum;
	currencyId = &petStruct->currencyId;

	callback = std::bind(&UupgradeBox::spawnPet, this);

	thumbnail = new Fimg("./images/pets/" + std::to_string(petStruct->id) + ".png");

	setup();
}

UupgradeBox::UupgradeBox(widget* parent, FupgradeStruct* upgradeStruct, FsaveUpgradeStruct* saveUpgradeStruct) {
	this->widgetParent = parent;

	this->upgradeStruct = upgradeStruct;
	this->saveUpgradeStruct = saveUpgradeStruct;

	nameString = upgradeStruct->name;
	descriptionString = upgradeStruct->description;

	upgradeNum = &saveUpgradeStruct->upgradeLevel;
	upgradeMax = upgradeStruct->upgradeNumMax;
	price = new double(upgrades::calcPrice(upgradeStruct, saveUpgradeStruct));
	currencyId = &upgradeStruct->currencyId;

	setup();
}

UupgradeBox::UupgradeBox(widget* parent, FvaultUnlocksStruct* vaultUnlocksStruct, FsaveVaultUnlocksStruct* saveVaultUnlocksStruct) {
	this->widgetParent = parent;

	this->vaultUnlocksStruct = vaultUnlocksStruct;
	this->saveVaultUnlocksStruct = saveVaultUnlocksStruct;

	nameString = vaultUnlocksStruct->name;
	descriptionString = vaultUnlocksStruct->description;

	unlocked = &saveVaultUnlocksStruct->unlocked;
	price = &vaultUnlocksStruct->currencyNum;
	currencyId = &vaultUnlocksStruct->currencyId;

	setup();
}

UupgradeBox::~UupgradeBox() {
	delete name;
	delete buyButton;
	delete buttonPriceText;
	delete upgradeText;
	name = nullptr;
	buyButton = nullptr;
	buttonPriceText = nullptr;
	upgradeText = nullptr;
}

void UupgradeBox::setup() {
	background = new Fimg("./images/widget/upgradeBoxBackground.png");

	thumbnailBackground = new Fimg("./images/widget/thumbnailBackground.png");
	name = new text(nameString, "straightDark", { 0, 0 });

	buyButton = new Ubutton(widgetParent, "widget/upgradeButton.png", 35, 13, 2, { 0, 0 }, false, false);
	buyButton->addCallback(this, &UupgradeBox::buyUpgrade);

	buttonPriceText = new text(shortNumbers::convert2Short(*price), "straightDark", { 0, 0 }, false, false, textAlign::center);
	buttonPriceText->setTextColor(255, 0, 0);

	currencyImg = new Fimg("./images/currency/coin" + std::to_string(*currencyId) + ".png");

	if (upgradeMax > 1) {
		upgradeText = new text(std::to_string(*upgradeNum) + "/" + std::to_string(upgradeMax), "straightDark", { 0, 0 }, false, false, textAlign::right);
	}

	if ((upgradeNum && *upgradeNum >= upgradeMax) || (unlocked && *unlocked)) {
		if (petStruct) {// compare to see what save pet
			if (SaveData::saveData.equippedPet.id == petStruct->id)
				buttonPriceText->setText("remove");
			else
				buttonPriceText->setText("equip");
		} else if (fishingRodStruct) {
			if (SaveData::saveData.equippedFishingRod.id == fishingRodStruct->id) {
				buttonPriceText->setText("equipped");
			} else if (saveFishingRodStruct->unlocked)
				buttonPriceText->setText("equip");
		} else if (baitStruct) {
			if (SaveData::saveData.equippedBait.id == baitStruct->id) {
				buttonPriceText->setText("remove");
			} else if (saveBaitStruct->unlocked)
				buttonPriceText->setText("equip");
		} else {
			buyButton->enable(false);
			//buyButton->setImg("./images/widget/upgradeButton3.png");
		}
	}

	//update();
}

void UupgradeBox::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	background->draw(renderer, false);

	thumbnailBackground->draw(renderer, false);
	if (thumbnail)
		thumbnail->draw(renderer, false);
	name->draw(renderer);
	buyButton->draw(renderer);

	if (upgradeStruct) {
		//int worldId = upgrades::worldNameToId(upgradeStruct->levelName);
		//if (*price <= SaveData::saveData.currencyList[worldId + 1].numOwned) // can afford
		if (*price <= SaveData::saveData.currencyList[*currencyId].numOwned) // can afford
			buttonPriceText->setTextColor(255, 255, 255); // set to og color
		else // cant afford
			buttonPriceText->setTextColor(255, 0, 0); // set red
	} else if (price) {
		//int worldId = upgrades::worldNameToId(Main::currWorld); // temp
		//if ((unlocked && *unlocked) || *price <= SaveData::saveData.currencyList[worldId + 1].numOwned) { // can afford
		if ((unlocked && *unlocked) || *price <= SaveData::saveData.currencyList[*currencyId].numOwned) { // can afford
			buttonPriceText->setTextColor(255, 255, 255); // set to og color
		} else // cant afford
			buttonPriceText->setTextColor(255, 0, 0); // set red
	}

	buttonPriceText->draw(renderer);
	// draw if not max or unlocked
	if ((!unlocked || (unlocked && !*unlocked)) && (!upgradeNum || (upgradeNum && *upgradeNum < upgradeMax)))
		currencyImg->draw(renderer, false);

	if (upgradeText)
		upgradeText->draw(renderer);
	// changes the text only if it isn't already set to current world
	if (mouseOver()) {
		NPCwidget* widget = dynamic_cast<NPCwidget*>(widgetParent);
		if (widget) {
			if (widget->name->getString() != nameString)
				widget->setNameDescription(nameString, descriptionString);
		} else if (UmerchantWidget* widget = dynamic_cast<UmerchantWidget*>(widgetParent)) {
			if (widget && widget->name->getString() != nameString)
				widget->setNameDescription(nameString, descriptionString);
		} else if (UfishermanWidget* widget = dynamic_cast<UfishermanWidget*>(widgetParent))
			if (widget && widget->name->getString() != nameString)
				widget->setNameDescription(nameString, buffString, debuffString);
	}
}

void UupgradeBox::setLocAndSize(vector loc, vector size) {
	__super::setLocAndSize(loc, size);
	setupLocs();
}

void UupgradeBox::setupLocs() {
	background->loc = loc;

	if (buyButton) {
		vector buttonSize = buyButton->getSize();
		vector buyButtonLoc = loc + vector{ size.x - buttonSize.x, size.y / 2 - buttonSize.y / 2 } + vector{ -10 * stuff::pixelSize, 0 };
		buyButton->setLoc(buyButtonLoc);

		if (buttonPriceText) {
			buttonPriceText->setLoc(buyButtonLoc + buyButton->getSize() / 2 - vector{ 0, buttonPriceText->getSize().y / 2 });
			if (currencyImg)
				currencyImg->loc = buttonPriceText->getLoc() + vector{ -currencyImg->getSize().x - 20 * stuff::pixelSize, buttonPriceText->getSize().y / 2 - currencyImg->getSize().y / 2 };
		}

		if (upgradeText) {
			vector upgradeTextLoc = { buyButtonLoc.x - 5 * stuff::pixelSize, loc.y + size.y / 2 - upgradeText->size.y / 2 };
			upgradeText->setLoc(upgradeTextLoc);
		}
	}

	thumbnailBackground->loc = loc + vector{ 4 * stuff::pixelSize, size.y / 2 - thumbnailBackground->h / 2 * stuff::pixelSize };
	
	if (thumbnail)
		thumbnail->loc = thumbnailBackground->loc;
	
	name->setLoc(loc + vector{ (thumbnailBackground->h + 6) * stuff::pixelSize, size.y / 2 - name->getSize().y / 2 });
}

bool UupgradeBox::mouseOver() {
	vector mousePos = Main::mousePos;

	if (!parent && mousePos.x >= loc.x && mousePos.x <= loc.x + size.x && mousePos.y >= loc.y && mousePos.y <= loc.y + size.y)
		return true;
	else if (parent) {
		vector parentLoc = parent->getLoc();
		vector parentSize = parent->getSize();
		if (mousePos.x >= loc.x + parentLoc.x && mousePos.x <= loc.x + size.x + parentLoc.x && mousePos.y >= loc.y + parentLoc.y && mousePos.y <= loc.y + size.y + parentLoc.y)
			// test if inside the rect too
			if (mousePos.x >= parentLoc.x && mousePos.x <= parentLoc.x + parentSize.x && mousePos.y >= parentLoc.y && mousePos.y <= parentLoc.y + parentSize.y)
				return true;
	}
	return false;
}

void UupgradeBox::buyUpgrade() {

	if (upgradeStruct) {
		//bool success = upgrades::upgrade(upgrades::getUpgrade(upgradeStruct->upgradeFunctionName), this);
		//std::cout << "upgradeStruct->upgradeFunctionName: " << upgradeStruct->upgradeFunctionName << std::endl;
		bool success = upgrades::upgrade(SaveData::data.upgradeData[upgrades::getUpgrade(upgradeStruct->upgradeFunctionName)->id], this, price);
		//*price = upgrades::calcPrice(upgradeStruct, saveUpgradeStruct);
	} else { // temp
		//int worldId = upgrades::worldNameToId(Main::currWorld); // temp
		// returns if the player doesn't have enough money
		if (price)
			std::cout << "price: " << *price << " > " << SaveData::saveData.currencyList[*currencyId].numOwned << std::endl;
		if ((!unlocked || !*unlocked) && (!price || *price > SaveData::saveData.currencyList[*currencyId].numOwned)) {
		//if (!unlocked || *unlocked || !price || *price > SaveData::saveData.currencyList[worldId + 1].numOwned) {
			std::cout << "returning" << std::endl;
			return;
		}
		
		if (upgradeNum) {
			if (*upgradeNum >= upgradeMax)
				return;

			(*upgradeNum)++;
				SaveData::saveData.currencyList[*currencyId].numOwned -= *price;

			if (upgradeText)
				upgradeText->setText(std::to_string(*upgradeNum) + "/" + std::to_string(upgradeMax));
		}

		// will equip the item or object once the layer has unlocked it, instead of needing to click twice
		if (savePetStruct && !savePetStruct->unlocked)
			spawnPet();
		else if (saveFishingRodStruct && !saveFishingRodStruct->unlocked)
			equipFishingRod();
		else if (saveBaitStruct && !saveBaitStruct->unlocked)
			equipBait();

		if ((upgradeNum && *upgradeNum >= upgradeMax) || (unlocked)) {
			if (unlocked) {
				if (!*unlocked) {
					*unlocked = true;
					SaveData::saveData.currencyList[*currencyId].numOwned -= *price;
				} else if (callback) {
					// equip / use
					callback();
				}
			}

			if (petStruct) {
				// since happening after function its gotta be reversed
				if (Main::pet && petStruct == Main::pet->getPetStruct()) {
					NPCwidget* npcWidget = dynamic_cast<NPCwidget*>(widgetParent);
					if (npcWidget)
						for (int i = 0; i < npcWidget->upgradeHolder->childList.size(); i++) {
							widget* child = npcWidget->upgradeHolder->childList[i].child;
							UupgradeBox* upgradeBox = dynamic_cast<UupgradeBox*>(child);
							if (upgradeBox && upgradeBox->savePetStruct->unlocked)
								upgradeBox->buttonPriceText->setText("equip");
						}

					buttonPriceText->setText("remove");
					// then remove pet
				} else {

					buttonPriceText->setText("equip");
				}
			} else if (fishingRodStruct) {
				// since happening after function its gotta be reversed
				if (fishingRodStruct->id == SaveData::saveData.equippedFishingRod.id) {
					UfishermanWidget* fishermanWidget = dynamic_cast<UfishermanWidget*>(widgetParent);
					if (fishermanWidget)
						for (int i = 0; i < fishermanWidget->upgradeHolderList[0]->childList.size(); i++) {
							widget* child = fishermanWidget->upgradeHolderList[0]->childList[i].child;
							UupgradeBox* upgradeBox = dynamic_cast<UupgradeBox*>(child);
							if (upgradeBox && upgradeBox->saveFishingRodStruct->unlocked)
								upgradeBox->buttonPriceText->setText("equip");
						}

					buttonPriceText->setText("equipped");
				} else {
					buttonPriceText->setText("equip");
				}
			} else if (baitStruct) {
				/*
				// since happening after function its gotta be reversed
				if (baitStruct->id == SaveData::saveData.equippedBait.id) {
					UfishermanWidget* fishermanWidget = dynamic_cast<UfishermanWidget*>(widgetParent);
					if (fishermanWidget)
						for (int i = 0; i < fishermanWidget->upgradeHolderList[1]->childList.size(); i++) {
							widget* child = fishermanWidget->upgradeHolderList[1]->childList[i].child;
							UupgradeBox* upgradeBox = dynamic_cast<UupgradeBox*>(child);
							if (upgradeBox && upgradeBox->saveBaitStruct->unlocked)
								upgradeBox->buttonPriceText->setText("equip");
						}

					buttonPriceText->setText("equipped");
				} else {
					buttonPriceText->setText("equip");
				}
				*/
				// since happening after function its gotta be reversed
				if (baitStruct->id == SaveData::saveData.equippedBait.id) {
					UfishermanWidget* fishermanWidget = dynamic_cast<UfishermanWidget*>(widgetParent);
					if (fishermanWidget)
						for (int i = 0; i < fishermanWidget->upgradeHolderList[1]->childList.size(); i++) {
							widget* child = fishermanWidget->upgradeHolderList[1]->childList[i].child;
							UupgradeBox* upgradeBox = dynamic_cast<UupgradeBox*>(child);
							if (upgradeBox && upgradeBox->saveBaitStruct->unlocked)
								upgradeBox->buttonPriceText->setText("equip");
						}

					buttonPriceText->setText("remove");
					// then remove pet
				} else {

					buttonPriceText->setText("equip");
				}

			} else {
				buyButton->enable(false);
				//buyButton->removeClickAnim();
				//buyButton->setImg("./images/widget/upgradeButton3.png");
				buttonPriceText->setText("Max");
				setupLocs(); // updates the locks of the button and the upgrade text
			}
		}

		Main::currencyWidget->updateList();
	}
}

void UupgradeBox::update() {
	if (upgradeText)
		upgradeText->setText(std::to_string(*upgradeNum) + "/" + std::to_string(upgradeMax));

	if (upgradeStruct) // set the price text
		buttonPriceText->setText(shortNumbers::convert2Short(saveUpgradeStruct->price));
	else
		buttonPriceText->setText(shortNumbers::convert2Short(*price));


	if (upgradeNum && *upgradeNum >= upgradeMax) { // if max
		buyButton->enable(false);
		//buyButton->removeClickAnim();
		//buyButton->setImg("./images/widget/upgradeButton3.png");
		buttonPriceText->setText("Max");
	}

}

void UupgradeBox::openWorld() {
	Main::openLevel(worldStruct->worldName);
}

void UupgradeBox::spawnPet() {
	// remove already existing pet
	// set new pet
	if (!Main::pet || Main::pet && petStruct->id != Main::pet->getPetStruct()->id) {
		delete Main::pet;
		Main::pet = nullptr;
		Main::pet = new Apet(petStruct, { 400, -200 });
	} else if (Main::pet && petStruct->id == Main::pet->getPetStruct()->id) {
		delete Main::pet;
		Main::pet = nullptr;
	}

	Main::heldFishWidget->updateList(Main::renderer);
}

void UupgradeBox::equipFishingRod() {
	if (SaveData::saveData.equippedFishingRod.id == -1 || fishingRodStruct->id != SaveData::saveData.equippedFishingRod.id)
		Main::character->equipFishingRod(fishingRodStruct);
}

void UupgradeBox::equipBait() {
	if (SaveData::saveData.equippedBait.id == -1 || baitStruct->id != SaveData::saveData.equippedBait.id)
		Main::character->equipBait(baitStruct);
	else {
		// unequip bait
		SaveData::saveData.equippedBait.id = -1;
	}
}

vector UupgradeBox::getSize() {
	return vector{ float(background->w), float(background->h) } * stuff::pixelSize;
}