#include "achievement.h"
#include "main.h"
#include "saveData.h"

void achievement::createAchievementList() {
	// fish number achievements
	achievements.push_back(std::make_unique<achievement>(0,
		[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 1; }));
	achievements.push_back(std::make_unique<achievement>(1,
		[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 100; }));
	achievements.push_back(std::make_unique<achievement>(2,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 10000; }));
	achievements.push_back(std::make_unique<achievement>(3,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 1000000; }));

	// money achievements
	achievements.push_back(std::make_unique<achievement>(4,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 1; }));
	achievements.push_back(std::make_unique<achievement>(5,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 100; }));
	achievements.push_back(std::make_unique<achievement>(6,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 10000; }));
	achievements.push_back(std::make_unique<achievement>(7,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 1000000; }));
}

void achievement::checkAchievements() {
	for (auto& achievement : achievements) {
		achievement->checkUnlock();
	}
}