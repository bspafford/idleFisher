#pragma once

#include "widget.h"

class text;

class UjournalProgressWidget : public widget {
public:
	UjournalProgressWidget(int fishNumTotal, int starsNumTotal, int sizeNumTotal);
	void draw(Shader* shaderProgram);
	void updateNumbers(int fishNum, int starsNum, int sizeNum);

private:
	Image* background;

	text* fishNumText;
	text* starsNumText;
	text* sizeNumText;
	text* fishNumTextNum;
	text* starsNumTextNum;
	text* sizeNumTextNum;

	int fishNumTotal, starsNumTotal, sizeNumTotal;
	int fishNum, starsNum, sizeNum;
};