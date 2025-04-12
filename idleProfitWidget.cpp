#include "idleProfitWidget.h"
#include "main.h"
#include "button.h"
#include "text.h"
#include "heldFishWidget.h"
#include "scrollBox.h"

UidleProfitWidget::UidleProfitWidget() {
	collectButton = new Ubutton(this, "widget/button.png", 54, 39, 2, { 0, 0 }, false, false);

	title = new text("Idle Profit", "straight", { 0, 0 }, false, false, textAlign::center);

	std::vector<std::string> collectButtonAnim = { "./images/widget/button1.png", "./images/widget/button2.png" };
	//collectButton->setClickAnim(collectButtonAnim);
	collectButton->addCallback<widget>(this, &widget::removeFromViewport);

	collectText = new text("Collect", "straight", { 0, 0 }, false, false, textAlign::center);

	scrollBox = new UscrollBox();
	heldFishWidget = new UheldFishWidget();

	setupLocs();
}

UidleProfitWidget::~UidleProfitWidget() {
	delete heldFishWidget;
	delete collectButton;
	delete collectText;
	heldFishWidget = nullptr;
	collectButton = nullptr;
	collectText = nullptr;
}

void UidleProfitWidget::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &background);
	title->draw(renderer);
	collectButton->draw(renderer);
	collectText->draw(renderer);

	return;
	scrollBox->draw(renderer);
}

void UidleProfitWidget::setup(std::vector<FsaveFishData> fishList) {
	heldFishWidget->updateList(Main::renderer, fishList);
	scrollBox->addChild(heldFishWidget, heldFishWidget->getSize().y);
	scrollBox->setLocAndSize({ .35f * Main::screenWidth, .1f * Main::screenHeight }, { 40 * stuff::pixelSize, .8f * Main::screenHeight });
	setupLocs();
}

void UidleProfitWidget::addedToViewport() {

}

void UidleProfitWidget::setupLocs() {
	background = { int(.35 * Main::screenWidth), int(.1 * Main::screenHeight), int(.3 * Main::screenWidth), int(.8 * Main::screenHeight)};

	title->setLoc(vector{ Main::screenWidth / 2.f, background.y + 5 * stuff::pixelSize });

	vector buttonSize = collectButton->getSize();
	collectButton->setLoc(vector{ Main::screenWidth / 2.f, .9f * Main::screenHeight } - vector{buttonSize.x / 2, buttonSize.y } - vector{ 0, 2 * stuff::pixelSize });
	//collectText->setLoc(vector{ Main::screenWidth / 2.f, .9f * Main::screenHeight } - vector{ 0, collectText->getSize().y } - vector{ 0, 2 * stuff::pixelSize });
	collectText->setLoc(collectButton->getLoc() + buttonSize / 2 - vector{0, collectText->getSize().y / 2}); // half button - half text.y

	scrollBox->setLoc(vector{Main::screenWidth / 2.f, Main::screenHeight / 2.f} - heldFishWidget->getSize() / 2);
}
