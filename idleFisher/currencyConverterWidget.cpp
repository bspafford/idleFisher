#include "currencyConverterWidget.h"

#include "main.h"
#include "npc.h"

#include "verticalBox.h"
#include "text.h"
#include "button.h"

UcurrencyConverterWidget::UcurrencyConverterWidget(npc* parent) {
	this->parent = parent;

	upgradeHolder = std::make_unique<UscrollBox>();
	closeButton = std::make_unique<Ubutton>(this, "widget/npcXButton.png", 11, 11, 1, vector{ 0, 0 }, false, false);
	closeButton->addCallback<widget>(this, &UcurrencyConverterWidget::removeFromViewport);
	npcImg = std::make_unique<Image>("./images/widget/npcButtons/atm.png", vector{ 0, 0 }, false);

	name = std::make_unique<text>(" ", "biggerStraight", vector{ 0,0 });
	description = std::make_unique<text>(" ", "straight", vector{ 0,0 });
	nameHolder = std::make_unique<verticalBox>();
	nameHolder->addChild(name.get(), 8 * stuff::pixelSize);
	nameHolder->addChild(description.get(), 4 * stuff::pixelSize);

	npcBackground = std::make_unique<Image>("./images/widget/npcBackground.png", vector{ 0, 0 }, false);
	infoBackground = std::make_unique<Image>("./images/widget/infoBackground.png", vector{ 0, 0 }, false);
	upgradeBackground = std::make_unique<Image>("./images/widget/upgradeBackground.png", vector{ 0, 0 }, false);

	setup();
}

UcurrencyConverterWidget::~UcurrencyConverterWidget() {

}

void UcurrencyConverterWidget::setup() {
	for (int i = 1; i < SaveData::data.currencyData.size() - 1; i++) {
		FcurrencyStruct* currData = &SaveData::data.currencyData[i];
		FsaveCurrencyStruct* currSaveData = &SaveData::saveData.currencyList[currData->id];
		if (currSaveData->unlocked && SaveData::saveData.currencyList[currData->id + 1].unlocked) { // if currency and next are unlocked
			std::unique_ptr<UcurrencyConverterBox> upgradeBox = std::make_unique<UcurrencyConverterBox>(this, currData, currSaveData);
			if (upgradeBox->buyButton)
				upgradeBox->buyButton->setParent(upgradeHolder.get());
			upgradeHolder->addChild(upgradeBox.get(), upgradeBox->getSize().y);
			currencyConverterBoxList.push_back(std::move(upgradeBox));
		}
	}

	setupLocs();
}

void UcurrencyConverterWidget::draw(Shader* shaderProgram) {
	__super::draw(shaderProgram);

	if (!visible)
		return;

	upgradeBackground->draw(shaderProgram);
	npcBackground->draw(shaderProgram);
	if (infoBackground)
		infoBackground->draw(shaderProgram);

	npcImg->draw(shaderProgram);

	nameHolder->draw(shaderProgram);
	upgradeHolder->draw(shaderProgram);

	closeButton->draw(shaderProgram);
}

void UcurrencyConverterWidget::setNameDescription(std::string nameString, std::string descriptionString) {
	name->setText(nameString);
	description->setText(descriptionString);

	// change nameHolder sizes
	nameHolder->changeChildHeight(name.get(), name->getSize().y + stuff::pixelSize);
}

void UcurrencyConverterWidget::setupLocs() {
	__super::setupLocs();

	float x = (npcBackground->w + 1) * stuff::pixelSize;
	float y = (npcBackground->h + 1) * stuff::pixelSize;
	vector size = vector{ x, 0 } + upgradeBackground->getSize();
	vector center = { stuff::screenSize.x / 2, stuff::screenSize.y / 2 };
	vector topLeft = center - size / 2;
	npcBackground->setLoc(topLeft);
	if (infoBackground)
		infoBackground->setLoc(topLeft + vector{ 0, y });
	upgradeBackground->setLoc(topLeft + vector{ x, 0 });

	vector npcBackgroundSize = npcBackground->getSize();
	vector npcSize = npcImg->getSize();
	npcImg->setLoc(npcBackground->getLoc() + vector{npcBackgroundSize.x / 2, npcBackgroundSize.y} - vector{npcSize.x / 2, npcSize.y} - vector{0, 3 * stuff::pixelSize});


	upgradeHolder->setLocAndSize(upgradeBackground->getLoc(), vector{float(upgradeBackground->w), float(upgradeBackground->h - 6)} *stuff::pixelSize);
	upgradeHolder->ogLoc = upgradeBackground->getLoc() + vector{4, 3} *stuff::pixelSize;

	vector closeButtonSize = closeButton->getSize();
	closeButton->setLoc({ float(upgradeBackground->getLoc().x + upgradeBackground->w * stuff::pixelSize - closeButtonSize.x / 2), float(upgradeBackground->getLoc().y - closeButtonSize.y / 2)});

	if (infoBackground) {
		nameHolder->setLocAndSize({ float(infoBackground->getLoc().x) + 6 * stuff::pixelSize, float(infoBackground->getLoc().y) + 6 * stuff::pixelSize}, vector{float(infoBackground->w), float(infoBackground->h)} *stuff::pixelSize);
		name->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
		description->setLineLength((infoBackground->w - 10) * stuff::pixelSize);
	}
}

void UcurrencyConverterWidget::addedToViewport() {
	std::cout << "added to viewport" << std::endl;

	// check to see if there are now more currencies unlocked
	// if so add the child
	// if not do nothing
	int unlockedNum = 0;
	//for (const FsaveCurrencyStruct currency : SaveData::saveData.currencyList) {
	for (int i = 1; i < SaveData::saveData.currencyList.size(); i++) {
		FsaveCurrencyStruct currency = SaveData::saveData.currencyList[i];
		std::cout << "currency: " << currency.id << ", " << currency.unlocked << std::endl;
		if (currency.unlocked)
			unlockedNum++;
	}

	int childListSize = upgradeHolder->childList.size();
	std::cout << "childListSize123: " << childListSize << ", " << unlockedNum << std::endl;
	if (childListSize != unlockedNum) {
		// add children needed
		// its possible for there to be multiple new currencies

		// loop through starting at i = childListSize - 1 to unlockedNum
		// then add child, child being currency[i]
		std::cout << "childListSize: " << childListSize << ", " << unlockedNum << std::endl;
		for (int i = childListSize; i < unlockedNum-1; i++) {
			FcurrencyStruct* currData = &SaveData::data.currencyData[i+1];
			FsaveCurrencyStruct* currSaveData = &SaveData::saveData.currencyList[currData->id];
			if (currSaveData->unlocked && SaveData::saveData.currencyList[currData->id + 1].unlocked) { // if currency and next are unlocked
				std::unique_ptr<UcurrencyConverterBox> upgradeBox = std::make_unique<UcurrencyConverterBox>(this, currData, currSaveData);
				if (upgradeBox->buyButton)
					upgradeBox->buyButton->setParent(upgradeHolder.get());
				std::cout << "adding child:" << std::endl;
				upgradeHolder->addChild(upgradeBox.get(), upgradeBox->getSize().y);
				currencyConverterBoxList.push_back(std::move(upgradeBox));
			}
		}
	}
	setupLocs();
}