#include "newRecordWidget.h"
#include "text.h"
#include "timer.h"
#include "main.h"

UnewRecordWidget::UnewRecordWidget() {
	recordText = new text("New Record!", "straight", { 0, 0 }, false, false, textAlign::center);
	startTimer = new timer();
	startTimer->addCallback(this, &UnewRecordWidget::finished);
	setVisibility(false);
}

void UnewRecordWidget::draw(SDL_Renderer* renderer) {
	__super::draw(renderer);

	if (!visible)
		return;

	if (recordText) {
		recordText->setLoc(Main::getScreenSize() / 2.f - vector{ 0, 40 * stuff::pixelSize });
		recordText->draw(renderer);
	}
}

void UnewRecordWidget::start(int fishSize) {
	recordText->setText("New Record; " + std::to_string(fishSize) + "in!");
	setVisibility(true);
	startTimer->start(3);
}

void UnewRecordWidget::finished() {
	setVisibility(false);
}