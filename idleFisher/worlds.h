#pragma once

#include "stuff.h"
#include "math.h"

#include <string>
#include <stdio.h>
#include <vector>

#include "Image.h"

#include "tree.h"

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
class AfishGod;

// npc buidings
class Abuilding;
class Ahouse;
class AmerchantShop;
class AmechanicHouse;
class ApetShop;
class Arock;

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
	~titleScreen();

	static inline titleScreen* currTitleScreen;

	void start();
	void draw(Shader* shaderProgram);

	// water stuff
	std::shared_ptr<Image> waterImg;

	std::shared_ptr<animation> fishermanDock;
	std::shared_ptr<Image> title;
	std::shared_ptr<animation> trees;

	std::shared_ptr<Ubutton> newGameButton;
	std::shared_ptr<Ubutton> continueButton;
	std::shared_ptr<Ubutton> exitButton;

	std::shared_ptr<URectangle> transitionBox;

	// button callback
	void newGame();
	void continueGame();
	void exit();

	void fadeToBlack();
	void openWorld();

	std::shared_ptr<timer> fadeTimer;
	float alpha;
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
	~world();
	
	static inline world* currWorld;

	void start();
	void draw(Shader* shaderProgram);
	void setupAutoFishers();

	void spawnFishTransporter();

	std::unique_ptr<Image> mapImg;
	std::unique_ptr<animation> mapAnim;
	std::vector<std::string> mapAnimList;
	std::unique_ptr<Aship> ship;

	std::vector<std::unique_ptr<AfishSchool>> fishSchoolList;

	void makeDrawLists();
	// npcs
	std::vector<npc*> npcList; // list of all npcs in the world
	std::unique_ptr<Asailor> sailor;
	std::unique_ptr<Afisherman> fisherman;
	std::unique_ptr<Aatm> atm;
	std::unique_ptr<Ascuba> scuba;
	std::unique_ptr<ApetSeller> petSeller;
	std::unique_ptr<Amerchant> merchant;
	std::unique_ptr<Amechanic> mechanic;
	std::unique_ptr<AfishTransporter> fishTransporter;

	// npc buildings
	std::vector<Abuilding*> buildingList; // list of all npcs in the world
	std::unique_ptr<Ahouse> house;
	std::unique_ptr<AmerchantShop> merchantShop;
	std::unique_ptr<AmechanicHouse> mechanicHouse;
	std::unique_ptr<ApetShop> petShop;
	std::vector<std::unique_ptr<Arock>> rockList;

	// trees
	std::vector< std::unique_ptr<Atree>> trees;
	void sortTreeList();

	void sortDraw(Shader* shaderProgram);

	std::vector<std::string> circleImgs;
	std::unique_ptr<Image> circle;
	std::unique_ptr<animation> circleAnim;

	std::vector<std::unique_ptr<AautoFisher>> autoFisherList;
	std::unique_ptr<buyAutoFisher> buyer;
	std::unique_ptr<dumpster> sellFish;

	vector spawnLoc;
	vector houseLoc;

	void spawnFishSchool();
	void removeFishSchool(AfishSchool* fishSchool);
	std::unique_ptr<timer> fishSchoolSpawnTimer;
	int maxFishSchoolNum = 5;

	// rain
	void startRain();
	void stopRain();
	void darkenScreen();
	std::unique_ptr<Arain> rain;
	std::unique_ptr<timer> rainStartTimer;
	std::unique_ptr<timer> rainDurationTimer;
	std::unique_ptr<timer> darkenScreenTimer;
	float darkenValue = 0;
	bool isRaining = false;
	float minRainStartTime = 30*60, maxRainStartTime = 60*60;
	float minRainDurationTime = 5*60, maxRainDurationTime = 10*60;

	// water stuff
	void renderWater();
	void finishedBeachAnim();
	std::unique_ptr<Image> waterImg;
	std::unique_ptr<animation> beachAnim;

	// pole list
	std::vector<std::unique_ptr<Image>> poleList;

	vector bankSellLoc;

protected:
	void setWorldChangeLoc(int worldChangeLoc);
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