#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <chrono>

#include "math.h"
#include "stuff.h"

#include "worlds.h"

#include "Image.h"
#include "pet.h"

class Texture;
class Shader;
class Model;
class Camera;
class Acharacter;
class animation;
class widget;
class AautoFisher;
class IHoverable;

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

struct Fcollision;

class Main {
public:
	int createWindow();
	static inline GLFWwindow* window;
	void Start();
	void Update(float deltaTime);
	void updateShaders(float deltaTime);
	void draw3D(Shader* shaderProgram);
	void draw(Shader* shaderProgram);
	static void drawWidgets(Shader* shaderProgram);
	static void drawFishingLine(Shader* shaderProgram);

	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	static void checkInputs();

	// shaders
	static inline Shader* shaderProgram;
	static inline Shader* shadowMapProgram;
	static inline Shader* twoDShader;
	static inline Shader* quadShader;
	static inline Shader* waterShader;
	static inline Shader* twoDWaterShader;

	// 3d
	Model* house;
	Model* characterModel;
	static inline Camera* camera;
	
	// 2d
	static void loadIdleProfits();

	// widgets
	void setupWidgets();
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

	// world
	// if overrideIfInWorld true, then it will load the world even if the player is in the curr world
	static void openLevel(std::string worldName, int worldChangeLoc = worldLoc::changeWorlds, bool overrideIfInWorld = false);
	static inline std::string currWorldName = "";
	static inline bool switchingWorld = false;
	static inline std::string prevWorld = "world1"; // the previous world before entering the vault

public:
	static void calcMouseImg();
	static void setMouseImg(std::string cursorName);

	static inline Acharacter* character;

	static inline std::unique_ptr<Apet> pet;

	// if mouse is over water
	static inline bool mouseOverWater = false;

	// water stuff
	static inline float waveSpeed = 0.03f;
	static inline float waveFactor = 0;
	static inline float tideFactor = 0;

	bool renderShadows = false;

	int fpsCap = 0; // 0 is not cap

	static void rebirth();
	static double calcRebirthCurrency();

	static inline std::vector<UpremiumBuffWidget*> premiumBuffList;
	static inline std::vector<achievement*> achievements;
	static void checkAchievements();

	static inline GLFWcursor* cursor;
	static inline std::string currCursor;
};