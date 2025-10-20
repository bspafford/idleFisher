#pragma once

#include "math.h"

class Shader;
class Ubutton;
struct FvaultUnlocksStruct;
struct FsaveVaultUnlocksStruct;

struct FvaultStuff {
	vector offset;
	Ubutton* button;
	FvaultUnlocksStruct* vaultUnlocks;
	FsaveVaultUnlocksStruct* saveVaultUnlocks;
};

class AvaultPlacedItems {
public:
	AvaultPlacedItems();
	static void draw(Shader* shaderProgram);
	static void sortDraw(Shader* shaderProgram);
	static void placeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	static void removeItem(FvaultUnlocksStruct* vaultUnlock, FsaveVaultUnlocksStruct* saveVaultUnlock);
	static void selectItem();
	static void putItemInStorage();

	static inline bool inPlacingMode = false;

private:
	static inline FvaultStuff* selectedItem;

	static inline std::vector<FvaultStuff> itemList;
	static inline std::vector<FvaultStuff> sortedItemList;
};