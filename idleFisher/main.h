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

#include "character.h"
#include "camera.h"

// widgets
#include "pauseMenu.h"
#include "fishComboWidget.h"
#include "heldFishWidget.h"
#include "currencyWidget.h"
#include "comboWidget.h"
#include "achievementWidget.h"
#include "journal.h"
#include "fishUnlocked.h"
#include "UIWidget.h"
#include "idleProfitWidget.h"
#include "comboOvertimeWidget.h"
#include "newRecordWidget.h"
#include "idleProfitWidget.h"
#include "comboOvertimeWidget.h"
#include "newRecordWidget.h"

class widget;
class achievement;

class Texture;
class Shader;
class Model;
class animation;
class widget;
class AautoFisher;
class IHoverable;

struct Fcollision;

class Main {
public:
	~Main();

	int createWindow();
	static inline GLFWwindow* window;
	void Start();
	void Update(float deltaTime);
	void updateShaders(float deltaTime);
	void draw3D(Shader* shaderProgram);
	void draw(Shader* shaderProgram);
	static void drawWidgets(Shader* shaderProgram);
	void setTaskbarIcon(GLFWwindow* window);

	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	static void checkInputs();

	static inline bool running = true;

	// shaders
	static inline Shader* shaderProgram;
	static inline Shader* shadowMapProgram;
	static inline Shader* twoDShader;
	static inline Shader* quadShader;
	static inline Shader* waterShader;
	static inline Shader* twoDWaterShader;

	// 3d
	std::unique_ptr<Model> house;
	std::unique_ptr<Model> characterModel;
	static inline std::unique_ptr<Camera> camera;
	
	// 2d
	static void loadIdleProfits();

	// widgets
	void setupWidgets();
	static inline std::unique_ptr<UpauseMenu> pauseMenu;
	static inline std::unique_ptr<UfishComboWidget> fishComboWidget;
	static inline std::unique_ptr<UheldFishWidget> heldFishWidget;
	static inline std::unique_ptr<UcurrencyWidget> currencyWidget;
	static inline std::unique_ptr<UcomboWidget> comboWidget;
	static inline std::unique_ptr<UachievementWidget> achievementWidget;
	static inline std::unique_ptr<Ujournal> journal;
	static inline std::unique_ptr<UfishUnlocked> fishUnlocked;
	static inline std::unique_ptr<UUIWidget> UIWidget;
	static inline std::unique_ptr<UidleProfitWidget> idleProfitWidget;
	static inline std::unique_ptr<UcomboOvertimeWidget> comboOvertimeWiget;
	static inline std::unique_ptr<UnewRecordWidget> newRecordWidget;

public:
	static inline std::unique_ptr<Acharacter> character;

	static inline std::unique_ptr<Apet> pet;

	// water stuff
	static inline float waveSpeed = 0.03f;
	static inline float waveFactor = 0;
	static inline float tideFactor = 0;

	bool renderShadows = false;

	int fpsCap = 0; // 0 is not cap

	static void rebirth();
	static double calcRebirthCurrency();

	static inline std::vector<UpremiumBuffWidget*> premiumBuffList;
};