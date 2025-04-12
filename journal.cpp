#include "journal.h"
#include "main.h"
#include "saveData.h"
#include "shortNumbers.h"
#include "upgrades.h"

#include "fishBox.h"
#include "button.h"
#include "text.h"
#include "timer.h"
#include "animation.h"
#include "progressBar.h"
#include "journalProgressWidget.h"

Ujournal::Ujournal() {
	background = new Fimg("./images/widget/journal.png");
	journalClosed = new Fimg("./images/widget/journalClosed.png");

	std::unordered_map < std::string, animDataStruct> anim;
	anim.insert({ "open", { {0, 0}, {5, 0}, .1, false} });
	journalOpenAnim = new animation("widget/journalOpen.png", 282, 240, anim, false, { 0, 0 });
	journalOpenAnim->setAnimation("open");

	journalTimer = new timer();
	journalTimer->addUpdateCallback(this, &Ujournal::moveAnim);
	journalTimer->addCallback(this, &Ujournal::finishJournalAnim);

	forwardButton = new Ubutton(this, "widget/fowardButton.png", 19, 12, 1, vector{ 0, 0 }, false, false);
	backButton = new Ubutton(this, "widget/backButton.png", 19, 12, 1, vector{ 0, 0 }, false, false);
	xButton = new Ubutton(this, "widget/xButton.png", 11, 11, 1, vector{ 0, 0 }, false, false);
	forwardButton->addCallback(this, &Ujournal::forwardPage);
	backButton->addCallback(this, &Ujournal::backwardPage);
	xButton->addCallback(this, &Ujournal::closeWidget);

	if (SaveData::data.worldData.size() >= 2) {
		worldName1 = new text(SaveData::data.worldData[0].name, "biggerStraightDark", vector{ 0, 0 }, false, false, textAlign::center);
		worldName2 = new text(SaveData::data.worldData[0].name, "biggerStraightDark", vector{ 0, 0 }, false, false, textAlign::center);

		worldProgress1 = new UprogressBar(false, 100, 3);
		worldProgress1->setForegroundColor({ 227, 120, 64, 255 }, 0);
		worldProgress1->addProgressBar(0, 3, { 232, 210, 75, 255 });
		worldProgress1->addProgressBar(0, 1, { 120, 158, 36, 255 });
		worldProgress2 = new UprogressBar(false, 100, 3);
		worldProgress2->setForegroundColor({ 227, 120, 64, 255 }, 0);
		worldProgress2->addProgressBar(0, 3, { 232, 210, 75, 255 });
		worldProgress2->addProgressBar(0, 1, { 120, 158, 36, 255 });

		journalProgressWidget = new UjournalProgressWidget(5, 15, 5);
	}

	for (int i = 0; i < SaveData::data.fishData.size(); i++) {
		FfishData* currFish = &SaveData::data.fishData[i];
		FsaveFishData* currSaveFish = &SaveData::saveData.fishData[i];

		UfishBox* fishBox = new UfishBox(this, currFish, currSaveFish);

		fishBoxList.push_back(fishBox);
	}

	// selected fish page
	fishThumbnail = new Fimg("");
	notesBackground = new Fimg("./images/widget/journalNotes.png", { 0, 0 });
	selectedFishName = new text(" ", "biggerStraightDark", { 0, 0 }, false, false, textAlign::center);
	selectedFishDescription = new text(" ", "straightDark", { 0, 0 });

	statsTitle = new text("Stats", "biggerStraightDark", { 0 , 0 }, false, false, textAlign::center);
	baseCurrency = new text("Base Currency", "straightDark", { 0, 0 });
	baseCurrencyNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	currency = new text("Currency", "straightDark", { 0, 0 });
	currencyNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	caught = new text("Caught", "straightDark", { 0, 0 });
	caughtNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	power = new text("Power", "straightDark", { 0, 0 });
	powerNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	speed = new text("Speed", "straightDark", { 0, 0 });
	speedNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	yellow = new text("Yellow", "straightDark", { 0, 0 });
	yellowNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	green = new text("Green", "straightDark", { 0, 0 });
	greenNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	probability = new text("Probability", "straightDark", { 0, 0 });
	probabilityNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);
	fishSize = new text("Biggest (Max: 0)", "straightDark", { 0, 0 });
	fishSizeNum = new text(" ", "straightDark", { 0, 0 }, false, false, textAlign::right);

	star1 = new Fimg("./images/emptyStar.png");
	star2 = new Fimg("./images/emptyStar.png");
	star3 = new Fimg("./images/emptyStar.png");

	setupLocs();
	updatePages();
}

Ujournal::~Ujournal() {
	delete background;
	delete forwardButton;
	delete backButton;
	delete xButton;
	delete worldName1;
	delete worldName2;
	delete worldProgress1;
	delete worldProgress2;

	for (int i = 0; i < fishBoxList.size(); i++)
		delete fishBoxList[i];
	fishBoxList.clear();

	// selected fish page
	delete selectedFish;
	delete selectedSaveFish;
	delete fishThumbnail;
	delete notesBackground;
	delete selectedFishName;
	delete selectedFishDescription;
	delete map;
	delete star1;
	delete star2;
	delete star3;

	delete baseCurrency;
	delete baseCurrencyNum;
	delete currency;
	delete currencyNum;
	delete caught;
	delete caughtNum;
	delete power;
	delete powerNum;
	delete speed;
	delete speedNum;
	delete yellow;
	delete yellowNum;
	delete green;
	delete greenNum;
	delete probability;
	delete probabilityNum;
	delete fishSize;
	delete fishSizeNum;
}

void Ujournal::draw(SDL_Renderer* renderer) {
	if (journalClosed && !journalTimer->finished())
		journalClosed->draw(renderer, false);
	if (journalOpenAnim && !journalOpenAnim->finished() && journalTimer->finished())
		journalOpenAnim->draw(renderer);

	//if (journalOpenAnim && journalOpenAnim->finished() && journalTimer->finished())

	if (!journalOpenAnim || !journalOpenAnim->finished() || !journalTimer->finished())
		return;

	background->draw(renderer, false);
	
	for (int i = 0; i < fishBoxList.size(); i++) {
		if (pageNum == -1)
			break;

		if (fishBoxList[i]->fishData->levelName == SaveData::data.worldData[pageNum].worldName || (SaveData::data.worldData.size() > pageNum && fishBoxList[i]->fishData->levelName == SaveData::data.worldData[pageNum].worldName))
			fishBoxList[i]->draw(renderer);
	}

	if (backButton)
		backButton->draw(renderer);
	if (xButton)
		xButton->draw(renderer);

	if (pageNum != -1) {
		if (forwardButton)
			forwardButton->draw(renderer);
		worldName1->draw(renderer);
		worldName2->draw(renderer);

		if (SaveData::saveData.worldList[pageNum].unlocked) {
			worldProgress1->draw(renderer);
			worldProgress2->draw(renderer);
		}

		if (worldProgress1->mouseOver()) {
			journalProgressWidget->draw(renderer);
			journalProgressWidget->updateNumbers(unlockedFish1, fishStars1, maxFishSize1);
		} else if (worldProgress2->mouseOver()) {
			journalProgressWidget->draw(renderer);
			journalProgressWidget->updateNumbers(unlockedFish2, fishStars2, maxFishSize2);
		}

	} else {
		fishThumbnail->draw(renderer, false);
		notesBackground->draw(renderer, false);
		selectedFishName->draw(renderer);
		selectedFishDescription->draw(renderer);
		if (map)
			map->draw(renderer, false);

		statsTitle->draw(renderer);
		baseCurrency->draw(renderer);
		baseCurrencyNum->draw(renderer);
		currency->draw(renderer);
		currencyNum->draw(renderer);
		caught->draw(renderer);
		caughtNum->draw(renderer);
		power->draw(renderer);
		powerNum->draw(renderer);
		speed->draw(renderer);
		speedNum->draw(renderer);
		yellow->draw(renderer);
		yellowNum->draw(renderer);
		green->draw(renderer);
		greenNum->draw(renderer);
		probability->draw(renderer);
		probabilityNum->draw(renderer);
		fishSize->draw(renderer);
		fishSizeNum->draw(renderer);

		star1->draw(renderer, false);
		star2->draw(renderer, false);
		star3->draw(renderer, false);
	}
}

void Ujournal::forwardPage() {
	pageNum++;
	int size = int(SaveData::data.worldData.size()) - 1;
	if (pageNum > size)
		pageNum = size;

	updatePages();
}

void Ujournal::backwardPage() {
	if (pageNum != -1) {
		pageNum--;
		if (pageNum < 0)
			pageNum = 0;
	} else {
		pageNum = tempPageNum;
	}

	selectedFish = nullptr;
	selectedSaveFish = nullptr;

	updatePages();
}

void Ujournal::updatePages() {
	if (SaveData::saveData.worldList.size() == 0)
		return;

	if (pageNum != -1) {
		std::string worldName = SaveData::data.worldData[pageNum].name;

		if (SaveData::saveData.worldList[pageNum].unlocked)
			worldName1->setText(worldName);
		else
			worldName1->setText("???");

		calcWorldPercentage(worldProgress1, worldProgress2, pageNum);

		bool world2Unlocked = SaveData::saveData.worldList[pageNum].unlocked;
		if (world2Unlocked && SaveData::data.worldData.size() > pageNum) // checks if in range, incase add world and its only on left page
			worldName2->setText(worldName);
		else if (!world2Unlocked)
			worldName2->setText("???");
		else
			worldName2->setText("");

		vector center = Main::getScreenSize() / 2;
		vector firstFish = center + vector{ -110, -67 } *stuff::pixelSize;
		vector increment = vector{ 48, 48 } * stuff::pixelSize;
		vector secondpageDist = { 130 * stuff::pixelSize, 0 };
		fishBoxList[pageNum * 10 + 1]->setLoc(firstFish);
		fishBoxList[pageNum * 10 + 2]->setLoc(firstFish + vector{ increment.x, 0 });
		fishBoxList[pageNum * 10 + 3]->setLoc(firstFish + vector{ 0, increment.y });
		fishBoxList[pageNum * 10 + 4]->setLoc(firstFish + vector{ increment.x, increment.y });
		fishBoxList[pageNum * 10 + 5]->setLoc(firstFish + vector{ increment.x / 2, increment.y * 2 });

		fishBoxList[pageNum * 10 + 6]->setLoc(firstFish + secondpageDist);
		fishBoxList[pageNum * 10 + 7]->setLoc(firstFish + vector{ increment.x, 0 } + secondpageDist);
		fishBoxList[pageNum * 10 + 8]->setLoc(firstFish + vector{ 0, increment.y } + secondpageDist);
		fishBoxList[pageNum * 10 + 9]->setLoc(firstFish + vector{ increment.x, increment.y } + secondpageDist);
		fishBoxList[pageNum * 10 + 10]->setLoc(firstFish + vector{ increment.x / 2, increment.y * 2 } + secondpageDist);
	} else {
		selectedFishName->setLoc(vector{ 256, 98 } * stuff::pixelSize); // 174 57
		selectedFishDescription->setLoc(vector{ 126+ 80, 150+40 } * stuff::pixelSize);
		fishThumbnail->w *= 3;
		fishThumbnail->h *= 3;
		fishThumbnail->loc = (vector{ 150 + 80 + 25, 83 + 41 + 27 } * stuff::pixelSize) - (vector{ float(fishThumbnail->w), float(fishThumbnail->h) } / 2.f * stuff::pixelSize);
		notesBackground->loc = vector{ 123 + 80, 140 + 41 } * stuff::pixelSize;
		selectedFishDescription->setLineLength(102 * stuff::pixelSize);

		statsTitle->setLoc(vector{ 256 + 129, 98 } * stuff::pixelSize);
		baseCurrency->setLoc(vector{ 254 + 80, 60 + 50 } *stuff::pixelSize);
		baseCurrencyNum->setLoc(vector{ 352 + 80, 60 + 50 } *stuff::pixelSize);
		currency->setLoc(vector{254 + 80, 68 + 50 } * stuff::pixelSize);
		currencyNum->setLoc(vector{ 352 + 80, 68 + 50 } * stuff::pixelSize);
		caught->setLoc(vector{ 254 + 80, 76 + 50 } * stuff::pixelSize);
		caughtNum->setLoc(vector{ 352 + 80, 76 + 50 } * stuff::pixelSize);
		power->setLoc(vector{ 254 + 80, 84 + 50 } * stuff::pixelSize);
		powerNum->setLoc(vector{ 352 + 80, 84 + 50 } * stuff::pixelSize);
		speed->setLoc(vector{ 254 + 80, 92 + 50 } * stuff::pixelSize);
		speedNum->setLoc(vector{ 352 + 80, 92 + 50 } * stuff::pixelSize);
		yellow->setLoc(vector{ 254 + 80, 100 + 50 } * stuff::pixelSize);
		yellowNum->setLoc(vector{ 352 + 80, 100 + 50 } * stuff::pixelSize);
		green->setLoc(vector{ 254 + 80, 108 + 50 } * stuff::pixelSize);
		greenNum->setLoc(vector{ 352 + 80, 108 + 50 } * stuff::pixelSize);
		probability->setLoc(vector{ 254 + 80, 116 + 50 } * stuff::pixelSize);
		probabilityNum->setLoc(vector{ 352 + 80, 116 + 50 } * stuff::pixelSize);
		fishSize->setLoc(vector{ 254 + 80, 124 + 50 } * stuff::pixelSize);
		fishSizeNum->setLoc(vector{ 352 + 80, 124 + 50 } * stuff::pixelSize);

		star1->loc = vector{ 174 + 58, 65 + 41 } *stuff::pixelSize;
		star2->loc = vector{ 174 + 74, 65 + 41 } *stuff::pixelSize;
		star3->loc = vector{ 174 + 90, 65 + 41 } *stuff::pixelSize;
	}
}

void Ujournal::openFishPage(FfishData* fishData, FsaveFishData* saveFishData) {
	if (pageNum != -1)
		tempPageNum = pageNum;
	pageNum = -1;

	
	Main::hoverNum = 0;
	Main::leaveHoverObject(NULL); // resets mouse cursor img

	selectedFish = fishData;
	selectedSaveFish = saveFishData;

	fishThumbnail->setImage(fishData->thumbnail);
	selectedFishName->setText(fishData->name);
	selectedFishDescription->setText(fishData->description);

	// find world name
	std::string worldName = "";
	for (FworldStruct world : SaveData::data.worldData) {
		if (world.worldName == fishData->levelName) {
			worldName = world.name;
		}
	}

	baseCurrencyNum->setText(shortNumbers::convert2Short(fishData->currencyNum));
	currencyNum->setText(shortNumbers::convert2Short(upgrades::getFishSellPrice(*fishData, 0)));
	caughtNum->setText(shortNumbers::convert2Short(saveFishData->calcTotalCaughtFish()));
	powerNum->setText(shortNumbers::convert2Short(fishData->fishingPower));
	speedNum->setText(shortNumbers::convert2Short(fishData->fishSpeed));
	yellowNum->setText(shortNumbers::convert2Short(fishData->yellowDifficulty));
	greenNum->setText(shortNumbers::convert2Short(fishData->greenDifficulty));
	probabilityNum->setText(shortNumbers::convert2Short(fishData->probability));
	fishSizeNum->setText(shortNumbers::convert2Short(saveFishData->biggestSizeCaught) + "in");
	fishSize->setText("Biggest (Max: " + std::to_string(fishData->maxSize) + "in)");

	delete map;
	map = new Fimg("./images/widget/maps/" + fishData->levelName + ".png");
	map->loc = vector{ 252+80, 134+46+6 } * stuff::pixelSize;

	if (saveFishData->totalNumOwned[1] != 0)
		star1->setImage("./images/widget/bronzeStar.png");
	if (saveFishData->totalNumOwned[2] != 0)
		star2->setImage("./images/widget/silverStar.png");
	if (saveFishData->totalNumOwned[3] != 0)
		star3->setImage("./images/widget/goldStar.png");

	updatePages();
}

void Ujournal::addedToViewport() {
	if (selectedFish && selectedSaveFish) {
		openFishPage(selectedFish, selectedSaveFish);
	} else
		updatePages();

	// update all the total num caught
	for (int i = 0; i < fishBoxList.size(); i++) {
		fishBoxList[i]->updateUI();
	}

	journalClosed->loc = Main::getScreenSize();
	journalTimer->start(.5);
}

void Ujournal::moveAnim() {
	/*
	vector start = Main::getScreenSize() * vector { .5, 1 } - background->getSize() * vector { .5, 0 } ;
	vector end = (Main::getScreenSize() - background->getSize()) / 2 ;

	float percent = journalTimer->getTime() / journalTimer->getMaxTime();
	float e = 1 - pow(1 - percent, 2.5);

	int x = math::lerp(start.x, end.x, e);
	int y = math::lerp(start.y, end.y, e);
	
	float pixelX = x - x % int(stuff::pixelSize);
	float pixelY = y - y % int(stuff::pixelSize);

	journalClosed->loc = { pixelX, pixelY - 28 * stuff::pixelSize };
	*/
	float percent = journalTimer->getTime() / journalTimer->getMaxTime();

	//journalClosed->loc = background->loc + vector{ 0, 100 };

	std::vector<vector> locs = { {0, 1000}, { 0, 500 }, {0, 100}, {0, 30}, {0, 10}, {0, 0}, {0, 0} };
	journalClosed->loc = background->loc + locs[floor(percent * 6)] - vector{ 0, 27 } * stuff::pixelSize;
}

void Ujournal::finishJournalAnim() {
	journalOpenAnim->start();
}

void Ujournal::closeWidget() {
	removeFromViewport();
}

void Ujournal::setupLocs() {
	__super::setupLocs();
	
	background->loc = (Main::getScreenSize() - background->getSize()) / 2;
	journalOpenAnim->setLoc(background->loc - vector{ 0, 28 * stuff::pixelSize });
	journalClosed->loc = (background->loc + Main::getScreenSize());

	forwardButton->setLoc(Main::getScreenSize() / 2 + vector{ 107, 71 } *stuff::pixelSize);
	backButton->setLoc(Main::getScreenSize() / 2 + vector{ -126, 71 } *stuff::pixelSize);
	xButton->setLoc(Main::getScreenSize() / 2 + vector{ 114, -84 } * stuff::pixelSize);

	worldName1->setLoc(Main::getScreenSize() / 2 + vector{ -63, -80 } * stuff::pixelSize);
	worldName2->setLoc(Main::getScreenSize() / 2 + vector{ 68, -80 } * stuff::pixelSize);

	worldProgress1->setLoc(Main::getScreenSize() / 2 + vector{ -63-50, -80+11 } * stuff::pixelSize);
	worldProgress2->setLoc(Main::getScreenSize() / 2 + vector{ 68-50, -80+11 } * stuff::pixelSize);
}

void Ujournal::calcWorldPercentage(UprogressBar* normalProgressBar, UprogressBar* rareProgressBar, int worldId) {
	unlockedFish1 = 0, fishStars1 = 0, maxFishSize1 = 0;
	unlockedFish2 = 0, fishStars2 = 0, maxFishSize2 = 0;
	unlockedTotal1 = 0, starsTotal1 = 0, SizeTotal1 = 0;
	unlockedTotal2 = 0, starsTotal2 = 0, SizeTotal2 = 0;

	for (FfishData fishData : SaveData::data.fishData) {
		FsaveFishData saveFishData = SaveData::saveData.fishData[fishData.id];
		if (fishData.levelName == SaveData::data.worldData[worldId].worldName) {
			if (!fishData.isRareFish) { //  && saveFishData.unlocked
				unlockedTotal1++;
				starsTotal1 += 3;
				SizeTotal1++;

				if (saveFishData.unlocked) {
					unlockedFish1++;
					fishStars1 += saveFishData.totalNumOwned[1] != 0;
					fishStars1 += saveFishData.totalNumOwned[2] != 0;
					fishStars1 += saveFishData.totalNumOwned[3] != 0;
					maxFishSize1 += saveFishData.biggestSizeCaught == fishData.maxSize;
				}
			} else {
				unlockedTotal2++;
				starsTotal2 += 3;
				SizeTotal2++;

				if (saveFishData.unlocked) {
					unlockedFish2++;
					fishStars2 += saveFishData.totalNumOwned[1] != 0;
					fishStars2 += saveFishData.totalNumOwned[2] != 0;
					fishStars2 += saveFishData.totalNumOwned[3] != 0;
					maxFishSize2 += saveFishData.biggestSizeCaught == fishData.maxSize;
				}
			}
		}
	}

	//std::cout << "regular: (" << unlockedFish1 << " + " << fishStars1 << " + " << maxFishSize1 << ") / " << total1 << std::endl;
	//std::cout << "rare   : (" << unlockedFish2 << " + " << fishStars2 << " + " << maxFishSize2 << ") / " << total2 << std::endl;
	std::cout << "unlockedFish: " << unlockedFish1 << " / " << unlockedTotal1 << std::endl;
	if (unlockedTotal1)
		normalProgressBar->setPercent(unlockedFish1 / unlockedTotal1, 0);
	else
		normalProgressBar->setPercent(0, 0);

	std::cout << "fishStars: " << fishStars1 << " / " << starsTotal1 << std::endl;
	if (starsTotal1)
		normalProgressBar->setPercent(fishStars1 / starsTotal1, 1);
	else
		normalProgressBar->setPercent(0, 1);

	std::cout << "maxFishSize: " << maxFishSize1 << " / " << SizeTotal1 << std::endl;
	if (SizeTotal1)
		normalProgressBar->setPercent(maxFishSize1 / SizeTotal1, 2);
	else
		normalProgressBar->setPercent(0, 2);

	

	if (unlockedTotal2)
		rareProgressBar->setPercent(unlockedFish2 / unlockedTotal2, 0);
	else
		rareProgressBar->setPercent(0, 0);

	std::cout << "fishStars: " << fishStars2 << " / " << starsTotal2 << std::endl;
	if (starsTotal2)
		rareProgressBar->setPercent(fishStars2 / starsTotal2, 1);
	else
		rareProgressBar->setPercent(0, 1);

	std::cout << "maxFishSize: " << maxFishSize2 << " / " << SizeTotal1 << std::endl;
	if (SizeTotal2)
		rareProgressBar->setPercent(maxFishSize2 / SizeTotal2, 2);
	else
		rareProgressBar->setPercent(0, 2);

}