#pragma once

#include <SDL.h>
#include <vector>
#include <functional>
#include <memory>

#include "math.h"

// foward declaration
class Acharacter;
class Ucamera;
class AautoFisher;
struct Fcollision;
class Apet;
//widgets
class widget;
class UpauseMenu;
class Usettings;
class UfishComboWidget;
class UheldFishWidget;
class UcurrencyWidget;
class UcomboWidget;
class UachievementWidget;
class Ujournal;
class UfishUnlocked;
class UUIWidget;
class UloadingScreen;
class UpremiumBuffWidget;
class UidleProfitWidget;
class UcomboOvertimeWidget;
class UnewRecordWidget;
class achievement;

class Main {
public:
	static inline bool running = false;

	void Start(SDL_Renderer* renderer);
	void Update(float deltaTime);

	static vector getScreenSize();

	void createWindow();

	void setupImages(SDL_Renderer* renderer);

	static void hoverObject(widget* widget);
	static void leaveHoverObject(widget* widget);
	static inline int hoverNum = 0;

	static inline SDL_Renderer* renderer;

	static inline vector mousePos;
	static void calcMouseImg();

	float timeScale = 1;

	static inline bool KEYS[322];

	// character
	static inline Acharacter* character;
	static inline Ucamera* camera;
	static inline vector move = { 0, 0 };

	static inline Apet* pet;

	static inline std::unique_ptr<Fimg> cursorImg;

	static inline int screenWidth = 1920;
	static inline int screenHeight = 1080;

	// draw
	void draw(SDL_Renderer* renderer);
	static void drawWidgets(SDL_Renderer* renderer);
	static void drawFishingLine(SDL_Renderer* renderer);

	// widgets
	void setupWidgets(SDL_Renderer* renderer);
	static inline widget* currWidget = NULL;
	static inline UpauseMenu* pauseMenu;
	static inline Usettings* settingsWidget;
	static inline UfishComboWidget* fishComboWidget;
	static inline UheldFishWidget* heldFishWidget;
	static inline UcurrencyWidget* currencyWidget;
	static inline UcomboWidget* comboWidget;
	static inline UachievementWidget* achievementWidget;
	static inline Ujournal* journal;
	static inline UfishUnlocked* fishUnlocked;
	static inline UUIWidget* UIWidget;
	static inline UidleProfitWidget* idleProfitWidget;
	static inline UcomboOvertimeWidget* comboOvertimeWiget;
	static inline UnewRecordWidget* newRecordWidget;

	static inline std::vector<Fcollision*> allCollision;

	// if mouse is over water
	static inline bool hoverWater = false;
	static inline bool mouseOverWater = false;

	// world
	// if overrideIfInWorld true, then it will load the world even if the player is in the curr world
	static void openLevel(std::string worldName, int worldChangeLoc = 1, bool overrideIfInWorld = false);
	static inline std::string currWorld = "";
	static inline std::string prevWorld = "world1"; // the previous world before entering the vault

	// mouse input
	static inline std::vector<std::function<void()>> leftClickList; // which items were clicked that frame
	// static
	static void addLeftClick(void (*callback) ());
	static void addLeftClick(std::function<void()> callback);
	// non static
	template <class T> static void addLeftClick(T* const object, void (T::* const callback) ()) {
		leftClickList.push_back(std::bind_front(callback, object));
	}
	static inline bool bLeftClick = false;
	static inline bool bRightClick = false;
	static inline bool bLeftMouseButtonDown = false;
	static inline bool bRightMouseButtonDown = false;
	static inline int mouseWheelDir = 0;

	// fade in from back at start
	bool fadeFromBlack = true;
	float blackFadeTime = 0;
	float blackFadeTimer = .75;
	float blackFadeY = 255;

	static void loadIdleProfits();

	static void rebirth();
	static double calcRebirthCurrency();

	static inline std::vector<UpremiumBuffWidget*> premiumBuffList;

	static inline std::vector<achievement*> achievements;
	static void checkAchievements();
};