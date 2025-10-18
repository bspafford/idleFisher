#pragma once

#include "widget.h"

class text;

class UjournalProgressWidget : public widget {
public:
	UjournalProgressWidget(widget* parent, int fishNumTotal, int starsNumTotal, int sizeNumTotal);
	void draw(Shader* shaderProgram);
	void updateNumbers(int fishNum, int starsNum, int sizeNum);

private:
	std::unique_ptr<Image> background;

	std::unique_ptr<text> fishNumText;
	std::unique_ptr<text> starsNumText;
	std::unique_ptr<text> sizeNumText;
	std::unique_ptr<text> fishNumTextNum;
	std::unique_ptr<text> starsNumTextNum;
	std::unique_ptr<text> sizeNumTextNum;

	int fishNumTotal, starsNumTotal, sizeNumTotal;
	int fishNum, starsNum, sizeNum;
};