#include "rebirthWidget.h"
#include "main.h"
#include "saveData.h"
#include "button.h"
#include "text.h"
#include "rebirthUnlock.h"
#include "shortNumbers.h"
#include "hoverBox.h"

UrebirthWidget::UrebirthWidget() {
	for (int i = 0; i < SaveData::data.rebirthData.size(); i++) {
		FrebirthStruct curr = SaveData::data.rebirthData[i];
		UrebirthUnlock* unlock = new UrebirthUnlock(this, i);
		rebirthButtonList.push_back(unlock);
	}

	xButton = new Ubutton(this, "autoFisher/moreUI/xButton.png", 11, 11, 1, { 0, 0 }, false, false);
	xButton->addCallback<widget>(this, &UrebirthWidget::removeFromViewport);

	rebirthButton = new Ubutton(this, "widget/button.png", 27, 13, 1, { 0, 0 }, false, false);
	rebirthText = new text("Rebirth", "straight", { 0, 0 }, false, false, textAlign::center);
	rebirthButton->addCallback(Main::rebirth);
	rebirthNum = new text(" ", "straight", { 0, 0 });

	hoverBox = new UhoverBox();

	setupLocs();
}

UrebirthWidget::~UrebirthWidget() {
	delete xButton;
	delete rebirthButton;
	delete rebirthText;
	delete rebirthNum;
	for (int i = 0; i < rebirthButtonList.size(); i++) {
		delete rebirthButtonList[i];
		rebirthButtonList[i] = nullptr;
	}
	rebirthButtonList.clear();
	xButton = nullptr;
	rebirthButton = nullptr;
	rebirthText = nullptr;
	rebirthNum = nullptr;
}

void UrebirthWidget::draw(SDL_Renderer* renderer) {
	// tests if the mouse is over a button, if it is then don't scroll
	bool mouseOverUnlock = isMouseOverUnlock();

	if (!Main::bLeftMouseButtonDown)
		wasOverButton = false;
	else if (mouseOverUnlock)
		wasOverButton = true;

	if (!mouseOverUnlock && !wasOverButton) {
		if (Main::bLeftMouseButtonDown) {
			if (!mouseDownPrev) {
				startLoc = scrollLoc;
				mouseStartPos = Main::mousePos;
			}
			scrolling();
		}
		mouseDownPrev = Main::bLeftMouseButtonDown;
	}

	// draw background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect background = SDL_Rect{ 0, 0, Main::screenWidth, Main::screenHeight };
	SDL_RenderFillRect(renderer, &background);

	int mouseOverRebirth = -1;

	// draw lines
	for (int i = 0; i < SaveData::data.rebirthData.size(); i++) {
		FrebirthStruct curr = SaveData::data.rebirthData[i];

		bool mouseOverUnlock = rebirthButtonList[i]->isMouseOver();
		if (mouseOverUnlock) {
			SDL_SetRenderDrawColor(renderer, 167, 94, 90, 255); // red
			mouseOverRebirth = i;
		} else
			SDL_SetRenderDrawColor(renderer, 242, 233, 211, 255); // white

		for (int j = 0; j < curr.prerequisites.size(); j++) {
			if (mouseOverUnlock && SaveData::saveData.rebirthList[curr.prerequisites[j]].unlocked)
				SDL_SetRenderDrawColor(renderer, 124, 127, 85, 255); // green

			/*
			// line goes from current to prerequisite
			vector loc1 = (rebirthButtonList[i]->getLoc() + rebirthButtonList[i]->getSize() / 2) / stuff::pixelSize;
			vector loc2 = (rebirthButtonList[curr.prerequisites[j]]->getLoc() + rebirthButtonList[curr.prerequisites[j]]->getSize() / 2) / stuff::pixelSize;

			SDL_RenderSetScale(renderer, stuff::pixelSize, stuff::pixelSize);
			SDL_RenderDrawLine(renderer, loc1.x, loc1.y, loc2.x, loc2.y);
			SDL_RenderSetScale(renderer, 1, 1);
			*/

			vector loc1 = (rebirthButtonList[i]->getLoc() + rebirthButtonList[i]->getSize() / 2);
			vector loc2 = (rebirthButtonList[curr.prerequisites[j]]->getLoc() + rebirthButtonList[curr.prerequisites[j]]->getSize() / 2);
			drawLine(renderer, loc1, loc2);
		}
	}

	for (int i = 0; i < rebirthButtonList.size(); i++) {
		rebirthButtonList[i]->draw(renderer);
	}

	if (mouseOverRebirth != -1) {
		hoverBox->setInfo(SaveData::data.rebirthData[mouseOverRebirth].name, SaveData::data.rebirthData[mouseOverRebirth].description, shortNumbers::convert2Short(SaveData::data.rebirthData[mouseOverRebirth].currencyNum));
		hoverBox->draw(renderer);
	}
	
	if (rebirthButton)
		rebirthButton->draw(renderer);
	if (rebirthText)
		rebirthText->draw(renderer);
	if (rebirthNum)
		rebirthNum->draw(renderer);
	if (xButton)
		xButton->draw(renderer);
	
	// when hover make the prerequisites either thicker or a different color
}

void UrebirthWidget::drawLine(SDL_Renderer* renderer, vector loc1, vector loc2) {
	vector start = loc1;
	vector end = loc2;

	vector negative{ 1, 1 };
	vector diff = { round((end.x - start.x) / stuff::pixelSize), round((end.y - start.y) / stuff::pixelSize) };
	if (diff.x < 0) {
		negative.x = -1;
		diff.x *= -1;
	}
	if (diff.y < 0) {
		negative.y = -1;
		diff.y *= -1;
	}

	// offset
	vector p1 = diff;
	vector p2 = { 0, 0 };

	float minX = math::min(0, diff.x);
	float maxX = math::max(0, diff.x);
	float minY = math::min(0, diff.y);
	float maxY = math::max(0, diff.y);

	int sizeX = maxX - minX;
	int sizeY = maxY - minY;

	float prevY = 0;
	float diffY;
	float x, y, a;

	if (sizeX > 0 && sizeY > 0) {
		SDL_Rect* points = new SDL_Rect[sizeX];
		for (int i = 1; i < sizeX + 1; i++) {
			x = minX + i;

			// linear
			float m = (p2.y - p1.y) / (p2.x - p1.x);
			float b = p1.y - m * p1.x;
			y = m * x + b;

			int yInt = round(y * negative.y);

			diffY = yInt - prevY;
			if (diffY < 1)
				diffY = 1;

			points[i - 1] = SDL_Rect{ int(round(x * negative.x) * stuff::pixelSize + start.x), int((yInt - (int)diffY) * stuff::pixelSize + start.y), int(stuff::pixelSize), int(stuff::pixelSize * diffY) };

			prevY = yInt;
		}
		// draw a straight line
		SDL_RenderFillRects(renderer, points, sizeX);
		if (points) {
			delete[] points;
			points = nullptr;
		}
	} else {
		if (diff.x < 1)
			diff.x = 1;
		else if (diff.y < 1)
			diff.y = 1;

		vector loc;
		if (start.x > end.x)
			loc.x = end.x;
		else
			loc.x = start.x;

		if (start.y > end.y)
			loc.y = end.y;
		else
			loc.y = start.y;

		SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), int(stuff::pixelSize * diff.x), int(stuff::pixelSize * diff.y) }; // int(round(minX)), int(round(minY))
		SDL_RenderFillRect(renderer, rect);
		delete rect;
	}
}

void UrebirthWidget::scrolling() {
	if (justOpened) {
		startLoc = { 0, 0 };
		mouseStartPos = Main::mousePos;
		justOpened = false;
	}
	vector diff = mouseStartPos - startLoc;
	scrollLoc.x = math::clamp(Main::mousePos.x - diff.x, -100 * stuff::pixelSize, Main::screenWidth);
	scrollLoc.y = math::clamp(Main::mousePos.y - diff.y, -100 * stuff::pixelSize, Main::screenHeight);
	
	// need to set location of all the children
	for (int i = 0; i < rebirthButtonList.size(); i++) {
		rebirthButtonList[i]->setLoc(scrollLoc + SaveData::data.rebirthData[i].loc);
	}
}

void UrebirthWidget::addedToViewport() {
	justOpened = true;
	update();
}

void UrebirthWidget::update() {
	rebirthNum->setText(shortNumbers::convert2Short(SaveData::saveData.rebirthCurrency));
}

void UrebirthWidget::setupLocs() {
	scrolling();

	if (xButton)
		xButton->setLoc(vector{ Main::screenWidth - xButton->getSize().x, 0 } + vector{ -2, 2 } * stuff::pixelSize);

	if (rebirthButton) {
		vector rebirthButtonSize = rebirthButton->getSize();
		rebirthButton->setLoc(vector{ float(Main::screenWidth), float(Main::screenHeight) } - rebirthButtonSize - vector{ 2, 2 } *stuff::pixelSize);
		rebirthText->setLoc(rebirthButton->getLoc() + rebirthButtonSize / 2 - vector{ 0, rebirthText->getSize().y / 2 });
	}
	rebirthNum->setLoc(vector{ 1, 1 } *stuff::pixelSize);
}

bool UrebirthWidget::isMouseOverUnlock() {
	for (UrebirthUnlock* unlock : rebirthButtonList)
		if (unlock->isMouseOver())
			return true;
	return false;
}