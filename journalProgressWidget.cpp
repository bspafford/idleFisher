#include "journalProgressWidget.h"
#include "main.h"
#include "text.h"

UjournalProgressWidget::UjournalProgressWidget(int fishNumTotal, int starsNumTotal, int sizeNumTotal) {
	this->fishNumTotal = fishNumTotal;
	this->starsNumTotal = starsNumTotal;
	this->sizeNumTotal = sizeNumTotal;

	background = new Fimg("./images/widget/journalTiny.png");

	fishNumText = new text("Unlocked", "straightDark", { 0, 0 });
	starsNumText = new text("Stars", "straightDark", { 0, 0 });
	sizeNumText = new text("Size", "straightDark", { 0, 0 });

	fishNumTextNum = new text("0/5", "straightDark", { 0, 0 });
	starsNumTextNum = new text("0/15", "straightDark", { 0, 0 });
	sizeNumTextNum = new text("0/5", "straightDark", { 0, 0 });
}

void UjournalProgressWidget::draw(SDL_Renderer* renderer) {
	background->loc = Main::mousePos - vector{ 0, background->getSize().y };
	background->draw(renderer, false);

	float offset = -9;
	fishNumTextNum->setLoc(Main::mousePos + vector{ 6, -20 + offset } *stuff::pixelSize);
	starsNumTextNum->setLoc(Main::mousePos + vector{ 6, -13 + offset } *stuff::pixelSize);
	sizeNumTextNum->setLoc(Main::mousePos + vector{ 6, -6 + offset } *stuff::pixelSize);

	// gets the biggest text to push back the text by
	float max = math::max(fishNumTextNum->getSize().x, starsNumTextNum->getSize().x);
	max = math::max(max, sizeNumTextNum->getSize().x);
	//std::cout << fishNumTextNum->getSize().x << ", " << starsNumTextNum->getSize().x << ", " << sizeNumTextNum->getSize().x << ", max: " << max << std::endl;

	fishNumText->setLoc(Main::mousePos + vector{ max + 7 * stuff::pixelSize, (-20 + offset) * stuff::pixelSize});
	starsNumText->setLoc(Main::mousePos + vector{ max + 7 * stuff::pixelSize, (-13 + offset) * stuff::pixelSize});
	sizeNumText->setLoc(Main::mousePos + vector{ max + 7 * stuff::pixelSize, (-6 + offset) * stuff::pixelSize});
	
	

	fishNumText->draw(renderer);
	starsNumText->draw(renderer);
	sizeNumText->draw(renderer);

	fishNumTextNum->draw(renderer);
	starsNumTextNum->draw(renderer);
	sizeNumTextNum->draw(renderer);
}

void UjournalProgressWidget::updateNumbers(int fishNum, int starsNum, int sizeNum) {
	this->fishNum = fishNum;
	this->starsNum = starsNum;
	this->sizeNum = sizeNum;

	std::string unlock = std::to_string(fishNum) + "/" + std::to_string(fishNumTotal);
	std::string stars = std::to_string(starsNum) + "/" + std::to_string(starsNumTotal);
	std::string size = std::to_string(sizeNum) + "/" + std::to_string(sizeNumTotal);
	if (fishNumTextNum->getString() != unlock)
		fishNumTextNum->setText(unlock);
	if (starsNumTextNum->getString() != stars)
		starsNumTextNum->setText(stars);
	if (sizeNumTextNum->getString() != size)
		sizeNumTextNum->setText(size);


}