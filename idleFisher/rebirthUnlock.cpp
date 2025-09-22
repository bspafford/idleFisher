#include "rebirthUnlock.h"
#include "saveData.h"
#include "shortNumbers.h"
#include "text.h"
#include "button.h"
#include "rebirthWidget.h"

UrebirthUnlock::UrebirthUnlock(widget* parent, int id) {
	this->parent = parent;
	rebirthInfo = &SaveData::data.rebirthData[id];
	saveRebirthInfo = &SaveData::saveData.rebirthList[id];

	background = new Image("./images/widget/rebirthIcons/background.png", { 0, 0 }, false);

	// removes "./images/" from the string
	std::string thumbnailPath = rebirthInfo->thumbnailPath;
	thumbnailPath.erase(0, 9);

	button = new Ubutton(parent, thumbnailPath, 16, 16, 1, { 0, 0 }, false, false);
	button->addCallback(this, &UrebirthUnlock::onClick);

	upgradeCost = new text(shortNumbers::convert2Short(rebirthInfo->currencyNum), "straight", { 0, 0 }, false, false, textAlign::center);
	if (!saveRebirthInfo->unlocked) {
		if (background)
			background->setColorMod(glm::vec4(75.f/255.f, 75.f/255.f, 75.f/255.f, 1.f));
		if (button && button->getButtonImg())
			button->getButtonImg()->setColorMod(glm::vec4(75.f / 255.f, 75.f / 255.f, 75.f / 255.f, 1.f));
	}

	if (saveRebirthInfo->unlocked)
		unlock();
}

UrebirthUnlock::~UrebirthUnlock() {
	delete button;
	delete upgradeCost;
	button = nullptr;
	upgradeCost = nullptr;
}

void UrebirthUnlock::draw(Shader* shaderProgram) {
	background->draw(shaderProgram);
	button->draw(shaderProgram);
	//upgradeCost->draw(shaderProgram);
}

void UrebirthUnlock::onClick() {
	if (!saveRebirthInfo->unlocked && prerequisitesMet() && SaveData::saveData.rebirthCurrency >= rebirthInfo->currencyNum) {
		SaveData::saveData.rebirthCurrency -= rebirthInfo->currencyNum;
		static_cast<UrebirthWidget*>(parent)->update();
		unlock();
	}
}

bool UrebirthUnlock::prerequisitesMet() {
	for (int id : rebirthInfo->prerequisites)
		if (!SaveData::saveData.rebirthList[id].unlocked)
			return false;
	return true;
}

void UrebirthUnlock::unlock() {
	saveRebirthInfo->unlocked = true;
	//button->setImg("./images/widget/xButton.png");
	upgradeCost->setVisibility(false);
	std::cout << "unlocked thingy" << std::endl;

	background->setColorMod(glm::vec4(1.f));
	button->getButtonImg()->setColorMod(glm::vec4(1.f));
}

bool UrebirthUnlock::isMouseOver() {
	return button->isMouseOver();
}

void UrebirthUnlock::setLoc(vector loc) {
	__super::setLoc(loc);

	if (button) {
		button->setLoc(loc);
		background->setLoc(button->loc + button->getSize() / 2 - background->getSize() / 2);
		upgradeCost->setLoc(loc + vector{ button->getSize().x / 2, button->getSize().y + stuff::pixelSize });
	}
}

vector UrebirthUnlock::getSize() {
	return button->getSize();
}