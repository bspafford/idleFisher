#include "comboOvertimeWidget.h"
#include "main.h"
#include "progressBar.h"
#include "timer.h"

UcomboOvertimeWidget::UcomboOvertimeWidget() {
	progressBar = std::make_unique<UprogressBar>(false, 45, 3, FprogressBarDir::right, true);
	setVisibility(false);

	setupLocs();
}

UcomboOvertimeWidget::~UcomboOvertimeWidget() {

}

void UcomboOvertimeWidget::draw(Shader* shaderProgram) {
	if (!visible)
		return;

	progressBar->draw(shaderProgram);
}

void UcomboOvertimeWidget::addTimer(timer* timer) {
	comboTimer = timer;
	comboTimer->addUpdateCallback(this, &UcomboOvertimeWidget::updateProgressBar);
}

void UcomboOvertimeWidget::updateProgressBar() {
	progressBar->setPercent(comboTimer->getTime() / comboTimer->getMaxTime());
}

void UcomboOvertimeWidget::setupLocs() {
	progressBar->loc = vector{ stuff::screenSize.x / 2.f, 12 * stuff::pixelSize } - vector{ progressBar->getSize().x / 2, 0 };
}