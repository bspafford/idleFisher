#include "premiumBuffWidget.h"

#include "main.h"
#include "timer.h"
#include "progressBar.h"
#include "UIWidget.h"
#include "math.h"
#include "hoverBox.h"

UpremiumBuffWidget::UpremiumBuffWidget(FgoldenFishStruct goldenFish) {
	this->goldenFish = goldenFish;

	lifeTimer = new timer();
	lifeTimer->addCallback(this, &UpremiumBuffWidget::timerFinished);
	lifeTimer->addUpdateCallback(this, &UpremiumBuffWidget::timerUpdate);
	lifeTimer->start(goldenFish.time);

	progressBar = new UprogressBar(false, 15, 15, FprogressBarDir::up, true);
	progressBar->setBackgroundColor({ 0, 0, 0, 0 });
	progressBar->setForegroundColor({ 0, 0, 0, 50 });

	img = new Image("./images/fish/premium.png", { 0, 0 }, false);
	hoverBox = new UhoverBox();
	hoverBox->setInfo(goldenFish.name, goldenFish.description);
}

void UpremiumBuffWidget::draw(Shader* shaderProgram) {
	progressBar->draw(shaderProgram);
	img->draw(shaderProgram);

	if (mouseOver())
		hoverBox->draw(shaderProgram);
}

void UpremiumBuffWidget::timerFinished() {
	// kill
	// remove buff
	for (int i = 0; i < Main::premiumBuffList.size(); i++) {
		if (this == Main::premiumBuffList[i]) {
			Main::premiumBuffList.erase(Main::premiumBuffList.begin() + i);
			//delete this;
		}
	}
	Main::UIWidget->setupLocs();
}

void UpremiumBuffWidget::timerUpdate() {
	progressBar->setPercent(lifeTimer->getTime() / lifeTimer->getMaxTime());
}

void UpremiumBuffWidget::setLoc(vector loc) {
	__super::setLoc(loc);
	progressBar->setLoc(loc);
	vector progressBarSize = progressBar->getSize();
	vector imgLoc = loc + progressBarSize / 2 - vector{ float(img->w), float(img->h) } / 2 * stuff::pixelSize;
	img->setLoc(imgLoc);
}

FgoldenFishStruct UpremiumBuffWidget::getGoldenFish() {
	return goldenFish;
}

vector UpremiumBuffWidget::getSize() {
	return vector{ 17, 17 } * stuff::pixelSize;
}