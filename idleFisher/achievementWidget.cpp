#include "achievementWidget.h"

#include "main.h"
#include "saveData.h"
#include "timer.h"

// widgets
#include "achievementBox.h"
#include "wrapBox.h"
#include "scrollBox.h"
#include "hoverBox.h"
#include "button.h"
#include "text.h"
#include "equippedWidget.h"

UachievementWidget::UachievementWidget() {
	//vector loc = { stuff::screenSize.x * .15f, stuff::screenSize.y * .2f };
	//vector size = { stuff::screenSize.x * .7f, stuff::screenSize.y * .6f };
	background = std::make_unique<Image>("./images/widget/achievementBackground.png", vector{ 0, 0 }, false);

	startTimeText = std::make_unique<text>("", "straight", vector{ 0, 0 });
	changeTextTimer = std::make_unique<timer>();
	changeTextTimer->addCallback(this, &UachievementWidget::updateText);
	updateText();
	//changeTextTimer->start(1);


	scrollBox = std::make_unique<UscrollBox>();
	equippedWidget = std::make_unique<UequippedWidget>();
	equippedWidget->parent = scrollBox.get();
	achievementHolder = std::make_unique<UwrapBox>(loc, size);
	//achievementHolder->parent = scrollBox;
	achievementText = std::make_unique<text>("Achievements - 0/500 (0%)", "straight", vector{ 0, 0 });
	hoverBox = std::make_unique<UhoverBox>();
	xButton = std::make_unique<Ubutton>(this, "widget/npcXButton.png", 11, 11, 1, vector{ 0, 0 }, false, false);
	xButton->addCallback(this, &UachievementWidget::closeWidget);
	if (scrollBox) {
		scrollBox->addChild(NULL, 2 * stuff::pixelSize);
		scrollBox->addChild(startTimeText.get(), 7 * stuff::pixelSize);
		scrollBox->addChild(equippedWidget.get(), equippedWidget->getSize().y);
		scrollBox->addChild(achievementText.get(), achievementText->getSize().y);
		scrollBox->addChild(achievementHolder.get(), 5.f); // achievementHolder->getOverflowSize().y
	}


	for (int i = 0; i < SaveData::data.achievementData.size(); i++) {
		UachievementBox* achievementBox = new UachievementBox(i);
		achievementBox->parent = achievementHolder.get();
		achievementHolder->addChild(achievementBox);
	}

	setupLocs();
}

UachievementWidget::~UachievementWidget() {

}

void UachievementWidget::draw(Shader* shaderProgram) {
	background->draw(shaderProgram);

	scrollBox->draw(shaderProgram);

	for (int i = 0; i < SaveData::data.achievementData.size(); i++) {
		widget* child = achievementHolder->getChildAt(i);
		if (child->mouseOver()) {
			if (SaveData::saveData.achievementList[i].unlocked) {
				hoverBox->setInfo(SaveData::data.achievementData[i].name, SaveData::data.achievementData[i].description);
				hoverBox->draw(shaderProgram);
			}// else
			//	hoverBox->setInfo("???", "???");
			break;
		}
	}

	std::string name, description;
	bool unlocked;
	if (equippedWidget->mouseOverSlot(name, description)) {
		hoverBox->setInfo(name, description);
		hoverBox->draw(shaderProgram);
	}

	xButton->draw(shaderProgram);
}

void UachievementWidget::addedToViewport() {
	int achievementsUnlocked = 0;
	for (FsaveAchievementStruct achievement : SaveData::saveData.achievementList) {
		if (achievement.unlocked)
			achievementsUnlocked++;
	}

	int totalAchievements = SaveData::saveData.achievementList.size();

	// displays 1 decimal or 0 if whole number
	std::string percentAchievements;
	float percent = double(achievementsUnlocked) / double(totalAchievements) * 100.f;
	if (percent == std::floor(percent))
		percentAchievements = std::format("{}", static_cast<int>(percent));
	else
		percentAchievements = std::format("{:.1f}", percent);

	achievementText->setText("Achievements - " + std::to_string(achievementsUnlocked) + "/" + std::to_string(totalAchievements) + " (" + percentAchievements + "%)");
}

void UachievementWidget::setupLocs() {
	__super::setupLocs();

	background->setLoc(vector{ stuff::screenSize.x / 2.f, stuff::screenSize.y / 2.f } - background->getSize() / 2);

	//vector scrollBoxLoc = background->loc + vector{ 10, 8 } * stuff::pixelSize;
	vector scrollBoxLoc = background->getLoc() + vector{11, 8} * stuff::pixelSize;
	//vector scrollBoxSize = vector{ (20 * 16 + 2) * stuff::pixelSize, background->getSize().y - 15 * stuff::pixelSize };
	vector scrollBoxSize = vector{ (17 * 25 + 2) * stuff::pixelSize, background->getSize().y - 15 * stuff::pixelSize };
	scrollBox->setOgLoc(scrollBoxLoc);
	achievementHolder->setOgLoc(scrollBoxLoc);
	achievementHolder->setSize({ scrollBoxSize.x, achievementHolder->getSize().y });
	scrollBox->changeChildHeight(achievementHolder.get(), achievementHolder->getOverflowSize().y);
	scrollBox->setLocAndSize(scrollBoxLoc, scrollBoxSize);

	if (xButton) {
		vector buttonSize = xButton->getSize();
		//vector buttonLoc = //vector{ stuff::screenSize.x * .85f - buttonSize.x / 2, stuff::screenSize.y * .2f - buttonSize.y / 2};
		vector buttonLoc = vector{ vector{background->getSize().x - 10 * stuff::pixelSize, 0} + background->getLoc() };
		xButton->setLoc(buttonLoc);
	}
}

void UachievementWidget::updateText() {
	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
	int64_t total = std::chrono::duration_cast<std::chrono::seconds>(currentTime - SaveData::startTime).count();

	const int secondsInMinute = 60;
	const int secondsInHour = 3600;
	const int secondsInDay = 86400;

	int days = total / secondsInDay;
	total %= secondsInDay;
	int hours = total / secondsInHour;
	total %= secondsInHour;
	int minutes = total / secondsInMinute;
	total %= secondsInMinute;
	int seconds = total;

	std::string timeString = "";
	if (days > 0)
		timeString.append(std::to_string(days) + " day ");
	if (hours > 0)
		timeString.append(std::to_string(hours) + " hrs ");
	if (minutes > 0)
		timeString.append(std::to_string(minutes) + " min ");
	timeString.append(std::to_string(seconds) + " sec");

	startTimeText->setText(timeString);
	changeTextTimer->start(1);
}

void UachievementWidget::closeWidget() {
	removeFromViewport();
}

void UachievementWidget::updateEquipmentWidget() {
	equippedWidget->update();
	
}

void UachievementWidget::updateAchievementIcon(int id) {
	widget* child = achievementHolder->getChildAt(id);
	UachievementBox* box = dynamic_cast<UachievementBox*>(child);
	if (box)
		box->updateAchievementImage();
}