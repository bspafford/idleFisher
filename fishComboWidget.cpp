#include "fishComboWidget.h"

#include <SDL_image.h>

#include "stuff.h"
#include "main.h"
#include "Acharacter.h"
#include "upgrades.h"
#include "petBuffs.h"
#include "baitBuffs.h"

#include "shake.h"

UfishComboWidget::UfishComboWidget(SDL_Renderer* renderer) {
	fishComboBorderImg = IMG_LoadTexture(renderer, fishComboBorderPath.c_str());
	SDL_QueryTexture(fishComboBorderImg, NULL, NULL, &fishComboBorderX, &fishComboBorderY);

	fishImg = IMG_LoadTexture(renderer, fishPath.c_str());
	SDL_QueryTexture(fishImg, NULL, NULL, &fishX, &fishY);

	shake = new Eshake(5);

}

UfishComboWidget::~UfishComboWidget() {
	if (fishComboBorderImg)
		SDL_DestroyTexture(fishComboBorderImg);
	if (fishImg)
		SDL_DestroyTexture(fishImg);
}

int UfishComboWidget::click(bool fishing) {
	// see if you caught good fish
	int combo = getCombo();
	return combo;
}

void UfishComboWidget::Start(FfishData fish, int quality) {
	currFish = fish;
	this->quality = quality;

	shake->start(Main::getScreenSize());

	updateComboSize();
	fishSpeed = upgrades::calcFishComboSpeed(currFish, quality);

	visible = true;
	fishMoveBack = false;
	hitWallNum = 0;
	fishLoc = 0;
	setupRandomCombo();
}

int UfishComboWidget::getCombo() {
	int fishX = fishRect.x + fishRect.w / 2;

	int minGreen = greenRect.x;
	int maxGreen = greenRect.x + greenRect.w;

	int minYellow = yellowRect.x;
	int maxYellow = yellowRect.x + yellowRect.w;

	if (fishX >= minGreen && fishX <= maxGreen)
		return 2;
	else if (fishX >= minYellow && fishX <= maxYellow)
		return 1;
	else
		return 0;
}

void UfishComboWidget::Update(float deltaTime) {
	if (!visible)
		return;

	float num = 1;
	if (fishMoveBack)
		num = -1;

	fishLoc += fishSpeed * deltaTime * num;

	if (fishLoc >= 1 && !fishMoveBack) {
		fishMoveBack = true;
		hitWallNum++;
	} else if (fishLoc <= 0 && fishMoveBack) {
		fishMoveBack = false;
		hitWallNum++;
	}

	if (hitWallNum > upgrades::calcMaxComboBounce()) {
		// didn't catch fish
		Main::character->comboExceeded();
	}
}

void UfishComboWidget::setupRandomCombo() {
	updateComboStartEnd();
	double num = (double)rand() / RAND_MAX;
	comboLoc = (int)math::lerp((float)comboStart, (float)comboEnd, (float)num);
}

void UfishComboWidget::updateComboStartEnd() {

	float comboHalf = fishComboBorderX * stuff::pixelSize / 2;
	comboStart = -comboHalf + (fishComboBorderX * yellowSize) / 2;
	comboEnd = comboHalf - (fishComboBorderX * yellowSize) / 2 - (fishComboBorderX * yellowSize * stuff::pixelSize);

	/*
	comboBackgroundHalf = backgroundSlot->GetSize().X / 2 - 10;
	comboStart.X = -comboBackgroundHalf + overlay->GetDesiredSize().X / 2;
	comboEnd.X = comboBackgroundHalf - overlay->GetDesiredSize().X / 2;
	*/
}

void UfishComboWidget::draw(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
	if (!visible)
		return;

	shake->updateShake({ float(screenWidth), float(screenHeight) });
	screenWidth = shake->getShakeLoc().x;
	screenHeight = shake->getShakeLoc().y;


	// get max and min for fish loc
	float maxFishX = fishComboBorderX * stuff::pixelSize / 2 - fishX * stuff::pixelSize / 2 + (screenWidth / 2 - (fishX * stuff::pixelSize) / 2) - stuff::pixelSize * 4;
	float minFishX = -fishComboBorderX * stuff::pixelSize / 2 + fishX * stuff::pixelSize / 2 + (screenWidth / 2 - (fishX * stuff::pixelSize) / 2) + stuff::pixelSize * 4;
	fishRect = getRect(screenWidth, screenHeight, fishX, fishY);
	fishRect.x = math::lerp(minFishX, maxFishX, fishLoc);

	// blue background
	SDL_SetRenderDrawColor(renderer, 92, 127, 153, 255);
	SDL_Rect backgroundRect = getRect(screenWidth, screenHeight, fishComboBorderX - 2, fishComboBorderY - 2);
	SDL_RenderFillRect(renderer, &backgroundRect);

	// yellow
	SDL_SetRenderDrawColor(renderer, 226, 255, 0, 255);
	yellowRect = getRect(screenWidth, screenHeight, yellowSize * fishComboBorderX, fishComboBorderY - 2);
	yellowRect.x = comboLoc + screenWidth / 2;
	SDL_RenderFillRect(renderer, &yellowRect);

	// green
	SDL_SetRenderDrawColor(renderer, 115, 255, 0, 255);
	greenRect = getRect(screenWidth, screenHeight, greenSize * fishComboBorderX, fishComboBorderY - 2);
	greenRect.x = comboLoc + screenWidth / 2.f + (yellowSize / 2.f - greenSize / 2.f) * fishComboBorderX * stuff::pixelSize;
	SDL_RenderFillRect(renderer, &greenRect);

	// border
	SDL_Rect border = getRect(screenWidth, screenHeight, fishComboBorderX, fishComboBorderY);
	SDL_RenderCopy(renderer, fishComboBorderImg, NULL, &border);

	// fish
	// flip sprite
	SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;
	if (fishMoveBack)
		flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	SDL_Point rect = { 0, 0 };
	SDL_RenderCopyEx(renderer, fishImg, NULL, &fishRect, 0, &rect, flip);
}

SDL_Rect UfishComboWidget::getRect(int screenX, int screenY, int sizeX, int sizeY) {
	int x = sizeX * stuff::pixelSize;
	int y = sizeY * stuff::pixelSize;
	return SDL_Rect{screenX / 2 - x / 2, int(screenY / 1.5 - y / 2), x, y };
}

void UfishComboWidget::updateComboSize() {
	greenSize = math::clamp(calcGreenSize(), 0, 1);
	yellowSize = math::clamp(calcYellowSize(), 0, 1);
}

float UfishComboWidget::calcGreenSize() {
	return upgrades::calcGreenFishingUpgrade() / 100.f * (1.f / currFish.greenDifficulty * SaveData::saveData.equippedFishingRod.power);
}

float UfishComboWidget::calcYellowSize() {
	return calcGreenSize() + upgrades::calcYellowFishingUpgrade() * 2.f / 100.f * (1.f / currFish.yellowDifficulty * SaveData::saveData.equippedFishingRod.power);
}