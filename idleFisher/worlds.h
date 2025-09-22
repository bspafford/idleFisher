#pragma once

#include "stuff.h"
#include "math.h"

#include <string>
#include <stdio.h>
#include <vector>

#include "Image.h"

class animation;
class timer;
class Ubutton;
class Shader;
class URectangle;

class AautoFisher;
class buyAutoFisher;
class dumpster;
class AfishSchool;
class Aship;
class ArebirthExit;

class npc;
class Asailor;
class Afisherman;
class Aatm;
class Ascuba;
class ApetSeller;
class Amerchant;
class Amechanic;
class Adecorator;
class AfishTransporter;
class Arain;
class Atree;
class AfishGod;

// npc buidings
class Abuilding;
class Ahouse;
class AmerchantShop;
class AmechanicHouse;
class ApetShop;

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

class titleScreen {
public:
	titleScreen();
	static void deconstructor();

	static void start();
	static void draw(Shader* shaderProgram);

	// water stuff
	static inline Image* waterImg;

	static inline animation* fishermanDock;
	static inline Image* title;
	static inline animation* trees;

	static inline Ubutton* newGameButton;
	static inline Ubutton* continueButton;
	static inline Ubutton* exitButton;

	static inline URectangle* transitionBox;

	// button callback
	static void newGame();
	static void continueGame();
	static void exit();

	static void fadeToBlack();
	static void openWorld();

	static inline timer* fadeTimer;
	static inline float alpha;
};

class vaultWorld {
public:
	vaultWorld();
	static void deconstructor();
	static void start();
	static void draw(Shader* shaderProgram);
	
private:
	static void leaveHouse();

	static inline Image* map;

	static inline Ubutton* houseDoor;

	// npc
	static inline Adecorator* decorator;
};

class rebirthWorld {
public:
	rebirthWorld();
	static void deconstructor();
	static void start();
	static void draw(Shader* shaderProgram);

	static inline void addAnim();
	static inline void removeAnim();
	static inline std::vector<animation*> waterRippleList;
	static inline timer* waterTimer;

	static inline AfishGod* fishGod;
	static inline Image* rebirthWorldImg;
	static inline ArebirthExit* rebirthExit;

	static inline vector charMoveDir;
	static inline vector charPrevMoveDir;

	static inline Image* doorGlow;

	// water stuff
	static inline Image* waterImg;

private:
	static inline float backgroundTime = 0;
};

class world {
public:
	world();

	static void deconstructor();
	static void start();
	static void draw(Shader* shaderProgram);
	static void setupAutoFishers();

	static void spawnFishTransporter();

	static inline Image* mapImg;
	static inline animation* mapAnim;
	static inline std::vector<std::string> mapAnimList;
	static inline Aship* ship;

	static inline std::vector<AfishSchool*> fishSchoolList;

	static void makeDrawLists();
	// npcs
	static inline std::vector<npc*> npcList; // list of all npcs in the world
	static inline Asailor* sailor;
	static inline Afisherman* fisherman;
	static inline Aatm* atm;
	static inline Ascuba* scuba;
	static inline ApetSeller* petSeller;
	static inline Amerchant* merchant;
	static inline Amechanic* mechanic;
	static inline AfishTransporter* fishTransporter;
	
	// npc buildings
	static inline std::vector<Abuilding*> buildingList; // list of all npcs in the world
	static inline Ahouse* house;
	static inline AmerchantShop* merchantShop;
	static inline AmechanicHouse* mechanicHouse;
	static inline ApetShop* petShop;

	// trees
	static inline std::vector<Atree*> trees;
	static void sortTreeList();

	static void sortDraw(Shader* shaderProgram);

	static inline std::vector<std::string> circleImgs;
	static inline Image* circle;
	static inline animation* circleAnim;

	static inline std::vector<AautoFisher*> autoFisherList;
	static inline buyAutoFisher* buyer;
	static inline dumpster* sellFish;

	static inline vector spawnLoc;
	static inline vector houseLoc;

	static void spawnFishSchool();
	static void removeFishSchool(AfishSchool* fishSchool);
	static inline timer* fishSchoolSpawnTimer;
	static inline int maxFishSchoolNum = 5;

	// rain
	static void startRain();
	static void stopRain();
	static void darkenScreen();
	static inline Arain* rain;
	static inline timer* rainStartTimer;
	static inline timer* rainDurationTimer;
	static inline timer* darkenScreenTimer;
	static inline float darkenValue = 0;
	static inline bool isRaining = false;
	static inline float minRainStartTime = 30*60, maxRainStartTime = 60*60;
	static inline float minRainDurationTime = 5*60, maxRainDurationTime = 10*60;

	// water stuff
	static void renderWater();
	static void finishedBeachAnim();
	static inline Image* waterImg;
	static inline animation* beachAnim;

	// pole list
	static inline std::vector<Image*> poleList;

	static inline vector bankSellLoc;

protected:
	static void setWorldChangeLoc(int worldChangeLoc);
};

class world1 : public world {
public:
	world1(int worldChangeLoc);
};

class world2 : public world {
public:
	world2(int worldChangeLoc);
};

class world3 : public world {
public:
	world3(int worldChangeLoc);
};

class world4 : public world {
public:
	world4(int worldChangeLoc);
};

class world5 : public world {
public:
	world5(int worldChangeLoc);
};

class world6 : public world {
public:
	world6(int worldChangeLoc);
};

class world7 : public world {
public:
	world7(int worldChangeLoc);
};

class world8 : public world {
public:
	world8(int worldChangeLoc);
};

class world9 : public world {
public:
	world9(int worldChangeLoc);
};

class world10 : public world {
public:
	world10(int worldChangeLoc);
};