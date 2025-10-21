#pragma once

#include <string>

struct worldLoc {
	// don't set the position of that character when opening level
	static inline int None = 0;
	// change the position of the character to the sailor when changing worlds
	static inline int changeWorlds = 1;
	// change the position of the character to the vault enterance when changing worlds
	static inline int leaveVault = 2;
	// set the character to position { 0, 0 }
	static inline int zero = 3;
};

class Scene {
public:
	static void openLevel(std::string worldName, int worldChangeLoc = worldLoc::changeWorlds, bool overrideIfInWorld = false);
	static int getWorldIndexFromName(std::string worldName);
	static std::string getPrevWorldName();
	static std::string getCurrWorldName();
private:
	static inline std::string prevWorld;
	static inline std::string currWorldName;
	static inline bool switchingScenes = false;
};