#include "UIWidget.h"
#include "main.h"
#include "saveData.h"
#include "shortNumbers.h"
#include "text.h"
#include "premiumBuffWidget.h"
#include "progressBar.h"
#include "button.h"
#include "worlds.h"
#include "timer.h"

#include "merchant.h"
#include "fisherman.h"
#include "mechanic.h"
#include "petSeller.h"
#include "sailor.h"
#include "atm.h"


UUIWidget::UUIWidget() {
	progressBar = new UprogressBar(false, 43, 6);
	progressBar->setBackgroundColor(SDL_Color(0, 0, 0, 255));
	progressBar->setForegroundColor(SDL_Color(0, 255, 255, 255));
	//progressBackground = new SDL_Rect{0, 0, int(43 * stuff::pixelSize), int(6 * stuff::pixelSize)};
	//progressBar = new SDL_Rect{ 0, 0, 0, int(6 * stuff::pixelSize)};
	currRunRebirthPoints = new text(" ", "straight", { 0, 0 }, false, false, textAlign::right);
	rebirthPointNum = new text(" ", "straight", { 0, 0 }, false, false, textAlign::right);


	NPCshowButton = new Ubutton(NULL, "widget/npcButtons/shopIcon.png", 26, 22, 1, { 0, 0 }, false, false);
	merchantButton = new Ubutton(NULL, "widget/npcButtons/npcButton1.png", 18, 18, 1, { 0, 0 }, false, false);
	sailorButton = new Ubutton(NULL, "widget/npcButtons/npcButton2.png", 18, 18, 1, { 0, 0 }, false, false);
	fishermanButton = new Ubutton(NULL, "widget/npcButtons/npcButton3.png", 18, 18, 1, { 0, 0 }, false, false);
	mechanicButton = new Ubutton(NULL, "widget/npcButtons/npcButton4.png", 18, 18, 1, { 0, 0 }, false, false);
	petSellerButton = new Ubutton(NULL, "widget/npcButtons/npcButton5.png", 18, 18, 1, { 0, 0 }, false, false);
	atmButton = new Ubutton(NULL, "widget/npcButtons/npcButton1.png", 18, 18, 1, { 0, 0 }, false, false);

	// set alpha to 0
	if (merchantButton) {
		if (merchantButton->getButtonImg())
			SDL_SetTextureAlphaMod(merchantButton->getButtonImg()->texture, 0);
		merchantButton->enable(false);
		merchantButton->addCallback(this, &UUIWidget::openMerchantWidget);
	}
	if (sailorButton) {
		if (sailorButton->getButtonImg())
			SDL_SetTextureAlphaMod(sailorButton->getButtonImg()->texture, 0);
		sailorButton->enable(false);
		sailorButton->addCallback(this, &UUIWidget::openSailorWidget);
	}
	if (fishermanButton) {
		if (fishermanButton->getButtonImg())
			SDL_SetTextureAlphaMod(fishermanButton->getButtonImg()->texture, 0);
		fishermanButton->enable(false);
		fishermanButton->addCallback(this, &UUIWidget::openFishermanWidget);
	}
	if (mechanicButton) {
		if (mechanicButton->getButtonImg())
			SDL_SetTextureAlphaMod(mechanicButton->getButtonImg()->texture, 0);
		mechanicButton->enable(false);
		mechanicButton->addCallback(this, &UUIWidget::openMechanicWidget);
	}
	if (petSellerButton) {
		if (petSellerButton->getButtonImg())
			SDL_SetTextureAlphaMod(petSellerButton->getButtonImg()->texture, 0);
		petSellerButton->enable(false);
		petSellerButton->addCallback(this, &UUIWidget::openPetSellerWidget);
	}
	if (atmButton) {
		if (atmButton->getButtonImg())
			SDL_SetTextureAlphaMod(atmButton->getButtonImg()->texture, 0);
		atmButton->enable(false);
		atmButton->addCallback(this, &UUIWidget::openAtmWidget);
	}

	if (NPCshowButton)
		NPCshowButton->addCallback(this, &UUIWidget::showNPCButtons);
	buttonsTimer = new timer();
	buttonsTimer->addUpdateCallback(this, &UUIWidget::updateButtonsLoc);
	buttonsTimer->addCallback(this, &UUIWidget::finishButtons);

	setupLocs();
}

UUIWidget::~UUIWidget() {
	//delete progressBackground;
	delete progressBar;
	delete currRunRebirthPoints;
	delete rebirthPointNum;
	delete merchantButton;
	delete fishermanButton;
	delete mechanicButton;
	delete petSellerButton;
	delete sailorButton;
	delete atmButton;
	progressBar = nullptr;
	currRunRebirthPoints = nullptr;
	rebirthPointNum = nullptr;
	merchantButton = nullptr;
	fishermanButton = nullptr;
	mechanicButton = nullptr;
	petSellerButton = nullptr;
	sailorButton = nullptr;
	atmButton = nullptr;
}

void UUIWidget::draw(SDL_Renderer* renderer) {
	progressBar->draw(renderer);
	currRunRebirthPoints->draw(renderer);
	rebirthPointNum->draw(renderer);

	int worldId = math::getWorldIndexFromName(Main::currWorld);
	if (worldId != -1) {
		if (atmButton && SaveData::saveData.npcSave[worldId].atmDiscovered)
			atmButton->draw(renderer);
		if (petSellerButton && SaveData::saveData.npcSave[worldId].petSellerDiscovered)
			petSellerButton->draw(renderer);
		if (mechanicButton && SaveData::saveData.npcSave[worldId].mechanicDiscovered)
			mechanicButton->draw(renderer);
		if (fishermanButton && SaveData::saveData.npcSave[worldId].fishermanDiscovered)
			fishermanButton->draw(renderer);
		if (sailorButton && SaveData::saveData.npcSave[worldId].sailorDiscovered)
			sailorButton->draw(renderer);
		if (merchantButton && SaveData::saveData.npcSave[worldId].merchantDiscovered)
			merchantButton->draw(renderer);
	}

	if (NPCshowButton)
		NPCshowButton->draw(renderer);

	for (int i = 0; i < Main::premiumBuffList.size(); i++) {
		Main::premiumBuffList[i]->draw(renderer);
	}
}

void UUIWidget::updateProgressBar(double points, double progress, double needed) {

	progressBar->setPercent(progress);

	std::string currPointString = shortNumbers::convert2Short(points);
	if (currRunRebirthPoints->getString() != currPointString)
		currRunRebirthPoints->setText(currPointString);
	std::string pointString = shortNumbers::convert2Short(SaveData::saveData.rebirthCurrency);
	if (rebirthPointNum->getString() != pointString)
		rebirthPointNum->setText(shortNumbers::convert2Short(SaveData::saveData.rebirthCurrency));
}

void UUIWidget::setupLocs() {
	vector size = progressBar->getSize();
	vector loc = { Main::screenWidth - size.x - 2 * stuff::pixelSize, 2 * stuff::pixelSize};
	progressBar->setLoc(loc);
	currRunRebirthPoints->setLoc(loc);
	rebirthPointNum->setLoc(vector{ Main::screenWidth - stuff::pixelSize, loc.y + rebirthPointNum->getSize().y + stuff::pixelSize });

	if (NPCshowButton) {
		NPCshowButton->setLoc({ 4 * stuff::pixelSize, Main::screenHeight - 4 * stuff::pixelSize - NPCshowButton->getSize().y });
		if (merchantButton)
			merchantButton->setLoc(NPCshowButton->getLoc());
		if (sailorButton)
			sailorButton->setLoc(NPCshowButton->getLoc());
		if (fishermanButton)
			fishermanButton->setLoc(NPCshowButton->getLoc());
		if (mechanicButton)
			mechanicButton->setLoc(NPCshowButton->getLoc());
		if (petSellerButton)
			petSellerButton->setLoc(NPCshowButton->getLoc());
		if (atmButton)
			atmButton->setLoc(NPCshowButton->getLoc());
	}

	float offset = Main::screenWidth;
	for (int i = 0; i < Main::premiumBuffList.size(); i++) {
		Main::premiumBuffList[i]->setLoc(vector{ offset, float(Main::screenHeight) } - Main::premiumBuffList[i]->getSize());
		offset -= Main::premiumBuffList[i]->getSize().x;
	}
}

void UUIWidget::showNPCButtons() {
	if (!canOpenClose)
		return;
	// show the buttons
	// make them slide out / animation
	// stay still, and make the buttons clickable?

	// maybe go away after not using them for a lil?
	canOpenClose = false;
	buttonsTimer->start(.5);
}

void UUIWidget::updateButtonsLoc() {
	// temp
	int worldId = math::getWorldIndexFromName(Main::currWorld);
	if (worldId == -1)
		return;
	
	buttonsList.clear();

	if (SaveData::saveData.npcSave[worldId].merchantDiscovered)
		buttonsList.push_back(merchantButton);
	if (SaveData::saveData.npcSave[worldId].sailorDiscovered)
		buttonsList.push_back(sailorButton);
	if (SaveData::saveData.npcSave[worldId].fishermanDiscovered)
		buttonsList.push_back(fishermanButton);
	if (SaveData::saveData.npcSave[worldId].mechanicDiscovered)
		buttonsList.push_back(mechanicButton);
	if (SaveData::saveData.npcSave[worldId].petSellerDiscovered)
		buttonsList.push_back(petSellerButton);
	if (SaveData::saveData.npcSave[worldId].atmDiscovered)
		buttonsList.push_back(atmButton);

	vector startLoc = NPCshowButton->getLoc() + vector{ 8 * stuff::pixelSize, 2 * stuff::pixelSize };
	float end = buttonsList.size() * 60.f + startLoc.x;
	for (int i = 0; i < buttonsList.size(); i++) {
		vector endLoc = vector{ 60.f * (i+1), 0 } + startLoc;
		if (buttonsList[i]->getLoc().x >= endLoc.x && !open) {
			buttonsList[i]->setLoc(endLoc - vector{ 0, stuff::pixelSize });
			continue;
		}

		float percent = buttonsTimer->getTime() / buttonsTimer->getMaxTime();
		if (open)
			percent = 1 - percent;

		// exponential
		float exp = percent * sqrt(percent); //percent * percent;

		exp = math::clamp(exp, 0, 1);
		float x = math::lerp(startLoc.x, end, exp);
		float y = startLoc.y;

		float distance = math::distance({ x, y }, endLoc);
		float distFromStart = 80;
		float startToEndDist = math::distance(startLoc, endLoc);
		float m = 1 / (startToEndDist - (startToEndDist - distFromStart));
		
		float newPercent = 1 - math::distance({ x, y }, endLoc) / math::distance(startLoc, endLoc);
		float temp = -m * ((startToEndDist - distFromStart) - newPercent * startToEndDist);
		temp = math::clamp(temp, 0, 1);

		if ((x >= endLoc.x && open)) {
			temp = 1;
		} else if ((x <= endLoc.x && open)) {
			buttonsList[i]->setLoc({ x, y });

		} else if (!open)
			buttonsList[i]->setLoc({ x, y });

		SDL_SetTextureAlphaMod(buttonsList[i]->getButtonImg()->texture, temp * 255);
	}
}

void UUIWidget::finishButtons() {
	canOpenClose = true;
	open = !open;

	if (open)
		for (int i = 0; i < buttonsList.size(); i++)
			buttonsList[i]->enable(true);
	else
		for (int i = 0; i < buttonsList.size(); i++)
			buttonsList[i]->enable(false);
}


void UUIWidget::openMerchantWidget() {
	world::merchant->click();
}

void UUIWidget::openFishermanWidget() {
	world::fisherman->click();
}

void UUIWidget::openMechanicWidget() {
	world::mechanic->click();
}

void UUIWidget::openPetSellerWidget() {
	world::petSeller->click();
}

void UUIWidget::openSailorWidget() {
	world::sailor->click();
}

void UUIWidget::openAtmWidget() {
	world::atm->click();
}