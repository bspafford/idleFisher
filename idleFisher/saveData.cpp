#include "saveData.h"

#include <iostream>

#include "main.h"
//#include "worlds.h"
//#include "AautoFisher.h"

//#include "merchant.h"
//#include "sailor.h"
//#include "fisherman.h"
//#include "mechanic.h"
//#include "petSeller.h"
//#include "atm.h"

SaveData::SaveData() {

}

void SaveData::save() {
	const auto filename = "./data/saves/save.save";

	// check if file exists
	std::ifstream is;
	bool saveStartTime = false;
	is.open(filename, std::ios::in | std::ios::binary);
	if (!is) {
		saveStartTime = true;
	}

	// Serialize to file
	std::ofstream os;
	os.open(filename, std::ios::out | std::ios::binary);

	if (saveStartTime) // save time if new save file
		os << math::getStringFromTime(std::chrono::system_clock::now()) << std::endl;
	else // save time from startTime if save file already exists
		os << math::getStringFromTime(startTime) << std::endl;
	os << math::getStringFromTime(std::chrono::system_clock::now()) << std::endl;

	getSaveInfo();

	std::vector<uint8_t> bytes;
	alpaca::serialize(saveData, bytes);

	// byte to string
	std::string text;
	for (uint8_t byte : bytes) {
		char c = (char)byte;
		std::string s(1, c);
		text.append(s);
	}

	os << text;

	os.close();
}

void SaveData::getSaveInfo() {
	std::cout << "saving: " << saveData.currWorld << ", " << saveData.prevWorld << std::endl;
	//saveData.currWorld = Main::currWorld;
	//saveData.prevWorld = Main::prevWorld;

	//for (int i = 0; i < world::autoFisherList.size(); i++)
	//	saveData.autoFisherList[i].fullness = world::autoFisherList[i]->calcCurrencyHeld();
}

void SaveData::load() {
	recalcLists();

	const auto filename = "./data/saves/save.save";

	std::ifstream is;
	is.open(filename, std::ios::in | std::ios::binary);
	if (!is) {
		save(); // makes save file if there isn't one
		startTime = std::chrono::system_clock::now(); // sets start time if no save file
		saveData.currWorld = "world1";
		saveData.prevWorld = "world1";
		return;
	}

	std::string line;
	std::getline(is, line);
	startTime = math::getTimeFromString(line);
	std::getline(is, line);
	lastPlayed = math::getTimeFromString(line);

	std::error_code ec;
	auto size = std::filesystem::file_size(filename);

	saveData = alpaca::deserialize<FsaveData>(is, size, ec);

	getLoadInfo();
}

void SaveData::getLoadInfo() {
	if (saveData.currWorld == "")
		saveData.currWorld = "world1";

	//Main::currWorld = saveData.currWorld;
	//Main::prevWorld = saveData.prevWorld;
}

template <typename T1, typename T2> static void recalcList(std::vector<T1>& data, std::vector<T2>& saveData) {
	// return if both lists are the same size
	if (saveData.size() == data.size())
		return;

	std::vector<T2> tempSaveData;
	for (int i = 0; i < data.size(); i++) {
		if (i <= (int)saveData.size() - 1) { // if saveData has that index then keep it
			tempSaveData.push_back(saveData[i]);
		} else { // else make new
			T2 temp;
			temp.id = data[i].id;
			tempSaveData.push_back(temp);
		}
	}

	saveData = tempSaveData;
}

void SaveData::recalcLists() {
	recalcList(data.fishData, saveData.fishData);
	for (int i = 0; i < saveData.fishData.size(); i++) {
		saveData.fishData[i].numOwned = std::vector<double>(4);
		saveData.fishData[i].totalNumOwned = std::vector<double>(4);
	}

	recalcList(data.currencyData, saveData.currencyList);
	recalcList(data.upgradeData, saveData.upgradeList);
	recalcList(data.worldData, saveData.worldList);
	recalcList(data.mechanicStruct, saveData.mechanicStruct);
	recalcList(data.autoFisherData, saveData.autoFisherList);
	recalcList(data.petData, saveData.petList);
	recalcList(data.vaultUnlockData, saveData.vaultUnlockList);
	recalcList(data.baitData, saveData.baitList);
	recalcList(data.buffData, saveData.buffList);
	recalcList(data.rebirthData, saveData.rebirthList);
	recalcList(data.achievementData, saveData.achievementList);
	// npcSave
	recalcList(data.worldData, saveData.npcSave);
	recalcList(data.currencyData, saveData.currencyConversionList);

	saveData.worldList[0].unlocked = true; // world 1 is always unlocked
	saveData.currencyList[1].unlocked = true; // currency 1 is always unlocked
}

void SaveData::saveSettings() {
	std::cout << "saving settings" << std::endl;

	const auto filename = "./data/saves/settings.save";

	// Serialize to file
	std::ofstream os;
	os.open(filename, std::ios::out | std::ios::binary);

	std::vector<uint8_t> bytes;
	alpaca::serialize(settingsData, bytes);

	// byte to string
	std::string text;
	for (uint8_t byte : bytes) {
		char c = (char)byte;
		std::string s(1, c);
		text.append(s);
	}

	os << text;

	os.close();
}

void SaveData::loadSettings() {
	const auto filename = "./data/saves/settings.save";

	std::ifstream is;
	is.open(filename, std::ios::in | std::ios::binary);
	if (!is) {
		saveSettings(); // makes save file if there isn't one
		return;
	}

	std::error_code ec;
	auto size = std::filesystem::file_size(filename);

	settingsData = alpaca::deserialize<FsettingsData>(is, size, ec);
}