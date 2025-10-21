#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <map>

#include "main.h"
#include "saveData.h"
#include "fishUnlocked.h"
#include "achievementWidget.h"

// achievement Class
class achievement {
private:
    FachievementStruct* achievementData;
    FsaveAchievementStruct* saveAchievementData;
    std::function<bool()> condition; // Condition as a lambda function
    static inline std::vector<achievement*> achievements;

public:
    achievement(int id,
        std::function<bool()> cond)
        : achievementData(&SaveData::data.achievementData[id]), saveAchievementData(&SaveData::saveData.achievementList[id]), condition(cond) {
    }

    void checkUnlock() {
        if (!saveAchievementData->unlocked && condition()) {
            saveAchievementData->unlocked = true;

            // updates the achievement widget icon for specific achievement
            Main::achievementWidget->updateAchievementIcon(saveAchievementData->id);

            notifyPlayer();
        }
    }

    void notifyPlayer() const {
        Main::fishUnlocked->start(achievementData);
    }

    bool isUnlocked() const { return saveAchievementData->unlocked; }

    FachievementStruct* getAchievementData() const { return achievementData; }

    static void createAchievementList();
    static void checkAchievements();
};