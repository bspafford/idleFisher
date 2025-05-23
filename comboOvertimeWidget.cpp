#include "comboOvertimeWidget.h"
#include "main.h"
#include "progressBar.h"
#include "timer.h"

UcomboOvertimeWidget::UcomboOvertimeWidget() {
	progressBar = new UprogressBar(false, 45, 3, FprogressBarDir::right, true);
	setVisibility(false);

	setupLocs();
}

UcomboOvertimeWidget::~UcomboOvertimeWidget() {
	delete progressBar;
	progressBar = nullptr;
}

void UcomboOvertimeWidget::draw(SDL_Renderer* renderer) {
	if (!visible)
		return;

	progressBar->draw(renderer);
}

void UcomboOvertimeWidget::addTimer(timer* timer) {
	comboTimer = timer;
	comboTimer->addUpdateCallback(this, &UcomboOvertimeWidget::updateProgressBar);
}

void UcomboOvertimeWidget::updateProgressBar() {
	progressBar->setPercent(comboTimer->getTime() / comboTimer->getMaxTime());
}

void UcomboOvertimeWidget::setupLocs() {
	progressBar->loc = vector{ Main::screenWidth / 2.f, 12 * stuff::pixelSize } - vector{ progressBar->getSize().x / 2, 0 };
}