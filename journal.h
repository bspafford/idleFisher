#pragma once

#include "widget.h"

class text;
class Ubutton;
class UfishBox;
class timer;
class animation;
class UprogressBar;
class UjournalProgressWidget;
struct FfishData;
struct FsaveFishData;

class Ujournal : public widget {
public:
	Ujournal();
	~Ujournal();

	void draw(SDL_Renderer* renderer);

	void openFishPage(FfishData* fishData, FsaveFishData* saveFishData);


private:
	void forwardPage();
	void backwardPage();
	void closeWidget();
	void updatePages();
	void addedToViewport() override;
	void setupLocs() override;

	void calcWorldPercentage(UprogressBar* normalProgressBar, UprogressBar* rareProgressBar, int worldId);

	Fimg* background;
	animation* journalOpenAnim;
	Fimg* journalClosed;

	timer* journalTimer;
	void moveAnim();
	void finishJournalAnim();

	Ubutton* forwardButton;
	Ubutton* backButton;
	Ubutton* xButton;
	text* worldName1;
	text* worldName2;

	UprogressBar* worldProgress1;
	UprogressBar* worldProgress2;

	UjournalProgressWidget* journalProgressWidget;
	int unlockedFish1 = 0, fishStars1 = 0, maxFishSize1 = 0;
	int unlockedFish2 = 0, fishStars2 = 0, maxFishSize2 = 0;
	float unlockedTotal1 = 0, starsTotal1 = 0, SizeTotal1 = 0;
	float unlockedTotal2 = 0, starsTotal2 = 0, SizeTotal2 = 0;

	std::vector<UfishBox*> fishBoxList;

	int pageNum = 0;
	int tempPageNum = 0; // used to get previous page when going back from selected fish

	// selected fish page
	FfishData* selectedFish;
	FsaveFishData* selectedSaveFish;
	Fimg* fishThumbnail;
	Fimg* notesBackground;
	text* selectedFishName;
	text* selectedFishDescription;

	text* statsTitle;
	text* baseCurrency;
	text* baseCurrencyNum;
	text* currency;
	text* currencyNum;
	text* caught;
	text* caughtNum;
	text* power;
	text* powerNum;
	text* speed;
	text* speedNum;
	text* yellow;
	text* yellowNum;
	text* green;
	text* greenNum;
	text* probability;
	text* probabilityNum;
	text* fishSize;
	text* fishSizeNum;

	Fimg* star1;
	Fimg* star2;
	Fimg* star3;

	Fimg* map;
};