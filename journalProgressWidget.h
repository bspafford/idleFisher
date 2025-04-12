#pragma once

#include "widget.h"

class text;

class UjournalProgressWidget : public widget {
public:
	UjournalProgressWidget(int fishNumTotal, int starsNumTotal, int sizeNumTotal);
	void draw(SDL_Renderer* renderer);
	void updateNumbers(int fishNum, int starsNum, int sizeNum);

private:
	Fimg* background;

	text* fishNumText;
	text* starsNumText;
	text* sizeNumText;
	text* fishNumTextNum;
	text* starsNumTextNum;
	text* sizeNumTextNum;

	int fishNumTotal, starsNumTotal, sizeNumTotal;
	int fishNum, starsNum, sizeNum;
};