#include "achievement.h"
#include "main.h"
#include "saveData.h"

void achievement::createAchievementList() {
	achievements = {

		// fish number achievements
		new achievement(0,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 1; }),
		new achievement(1,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 100; }),
		new achievement(2,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 10000; }), // 10000
		new achievement(3,
			[]() { return SaveData::saveData.fishData[1].calcTotalCaughtFish() >= 1000000; }), // 1000000

		// money achievements
		new achievement(4,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 1; }),
		new achievement(5,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 100; }),
		new achievement(6,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 10000; }),
		new achievement(7,
			[]() { return SaveData::saveData.currencyList[1].totalNumOwned >= 1000000; })
	};
}

void achievement::checkAchievements() {
	for (auto& achievement : achievements) {
		achievement->checkUnlock();
	}
}