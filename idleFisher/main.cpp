#include "main.h"
#include "Image.h"
#include "model.h"
#include "csvReader.h"
#include "sounds.h"
#include "stuff.h"
#include "timer.h"
#include "fps.h"
#include "Texture.h"
#include "textureManager.h"
#include "character.h"
#include "camera.h"
#include "AautoFisher.h"
#include "collision.h"
#include "AStar.h"
#include "upgrades.h"
#include "achievement.h"
#include "achievementBuffs.h"
#include "Input.h"
#include "Scene.h"
#include "Cursor.h"
#include "GPULoadCollector.h"

// npc
#include "fishTransporter.h"
#include "pet.h"
#include "atm.h"

// widgets
#include "widget.h"
#include "pauseMenu.h"
#include "settings.h"
#include "fishComboWidget.h"
#include "heldFishWidget.h"
#include "currencyWidget.h"
#include "comboWidget.h"
#include "achievementWidget.h"
#include "journal.h"
#include "fishUnlocked.h"
#include "UIWidget.h"
#include "premiumBuffWidget.h"
#include "idleProfitWidget.h"
#include "comboOvertimeWidget.h"
#include "newRecordWidget.h"

int main(int argc, char* argv[]) {
	srand(time(0));
	math::randRange(0, 100); // steups up srand i guess? otherwise first value always the same

	Main* _main = new Main();
	_main->createWindow();

	return 0;
}

int Main::createWindow() {
	GPULoadCollector::setMainThread(std::this_thread::get_id());

	fpsCap = 0;
	// temp
	stuff::screenSize = { 1920, 1080 };

	// Initialize GLFW
	glfwInit();

	// full screen
	if (false) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Set no decorations
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent resizing
	}

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	window = glfwCreateWindow(stuff::screenSize.x, stuff::screenSize.y, "Idle Fisher", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;						
		glfwTerminate();
		return -1;
	}

	// temp
	glfwSetWindowPos(window, 0, 0);

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	GLFWimage iconImgs[1];
	iconImgs[0].pixels = stbi_load("./images/icon.png", &iconImgs[0].width, &iconImgs[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, iconImgs);
	stbi_image_free(iconImgs[0].pixels);

	//Load GLAD so it configures OpenGL
	if (!gladLoadGL(glfwGetProcAddress)) {
		return -1;
	}

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, stuff::screenSize.x, stuff::screenSize.y);
	// Enables the Depth Buffer
	//glEnable(GL_DEPTH_TEST);

	// Enables Multisampling
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Start
	Start();

	// Framebuffer for Shadow Map
	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture for Shadow Map FBO
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Load in a model for shadows
	house = new Model("./images/models/idleFisher3D/idleFisher3DNoWater.gltf");
	house->setPos(glm::vec3(-182.75f, 0.f, -504.5f));
	house->setScale(glm::vec3(1.89f));
	house->setColor(glm::vec3(255, 240, 240));
	characterModel = new Model("./images/models/character/character.gltf");

	characterModel->setScale(glm::vec3(3.f));

	auto lastTime = std::chrono::steady_clock::now();

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		auto currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		//std::cout << "fps: " << 1.f / deltaTime << std::endl;
		
		// process input
		Input::pollEvents();

		checkInputs();
		Update(deltaTime);
		updateShaders(deltaTime);

		glViewport(0, 0, stuff::screenSize.x, stuff::screenSize.y);
		//glClearColor(.25, .6, .6, 1.f);
		glClearColor(18.f / 255.f, 11.f / 255.f, 22.f / 255.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Compute full light-space matrix
		float size = 300.f; // 100.f;// 35
		glm::vec3 lightPos = glm::vec3(-30.f, 100.f, 30.f) + camera->Position - glm::vec3(52.7046, 24.8073, 88.9249); // should follow camera pos
		glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, -1.f, 300.0f);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos - glm::vec3(-1, 1, 1), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView; // This is what you send to shaders

		renderShadows = false;
		// === SHADOW PASS (Render to Shadow Map) ===
		if (renderShadows) {
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, shadowMapWidth, shadowMapHeight);

			shadowMapProgram->Activate();
			shadowMapProgram->setMat4("lightSpaceMatrix", lightSpaceMatrix);

			// Render objects (WITHOUT camera)
			draw3D(shadowMapProgram);
			characterModel->Draw(shadowMapProgram, *camera);

			// Unbind FBO
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glViewport(0, 0, stuff::screenSize.x, stuff::screenSize.y);
		glEnable(GL_DEPTH_TEST);
		
		// === MAIN RENDER PASS (Render Scene with Shadows) ===

		characterModel->setPos(camera->Position + glm::vec3(-1.f, -.82f, -1.f) * glm::vec3(62.5f) + glm::vec3(9, 0, 9));

		glDisable(GL_DEPTH_TEST);
		// ==== DRAW 2D STUFF ====
		twoDShader->Activate();
		draw(twoDShader);

		glEnable(GL_DEPTH_TEST);
		// ==== DRAW SHADOW MESH ====
		shaderProgram->Activate();
		camera->Matrix(shaderProgram, "camMatrix");
		shaderProgram->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		if (renderShadows) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, shadowMap);
			shaderProgram->setInt("shadowMap", 1);
			shaderProgram->setInt("shadowOnly", 1);
			draw3D(shaderProgram);
		}

		Input::fireHeldInputs();
		glfwSwapBuffers(window);

		// checks for errors
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL Error: " << err << std::endl;
		}

		if (fpsCap != 0) {
			const auto cappedFPS = std::chrono::milliseconds(int(1000.f / float(fpsCap)));
			std::this_thread::sleep_until(currentTime + cappedFPS);
		}
	}

	// Delete all the objects we've created
	shaderProgram->Delete();
	twoDShader->Delete();
	shadowMapProgram->Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void Main::Start() {
	// Generates Shader object using shaders default.vert and default.frag
	shaderProgram = new Shader("default.vert", "default.frag");
	shadowMapProgram = new Shader("shadowMap.vert", "shadowMap.frag");
	twoDShader = new Shader("2dShader.vert", "2dShader.frag");
	quadShader = new Shader("test.vert", "test.frag");
	waterShader = new Shader("waterShader.vert", "waterShader.frag");
	twoDWaterShader = new Shader("2dWaterShader.vert", "2dWaterShader.frag");
	shaderProgram->Activate();

	// setup callbacks for input
	glfwSetFramebufferSizeCallback(window, windowSizeCallback);
	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetScrollCallback(window, Input::scrollCallback);
	glfwSetCursorPosCallback(window, Input::cursorPosCallback);

	SaveData::saveData.playerLoc = vector{ 1800, 1300 } / 3.f;
	SaveData::saveData.playerLoc = vector{ 663, 552 };

	textureManager::textureManager();
	sounds::sounds();
	csvReader();
	SaveData::load();
	upgrades::init();
	achievementBuffs::init();
	setupWidgets();

	Scene::openLevel("titleScreen", worldLoc::None, true);
	
	character = new Acharacter();
	camera = new Camera(stuff::screenSize.x, stuff::screenSize.y, glm::vec3(-55, 50, -350));

	// setup projection mat
	twoDShader->Activate();
	glUniformMatrix4fv(glGetUniformLocation(twoDShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjectionMat()));

	if (SaveData::saveData.equippedPet.id != -1)
		Main::pet = std::make_unique<Apet>(&SaveData::saveData.equippedPet, vector{ 400, -200 });

	fps::fps();

	achievement::createAchievementList();
}

void Main::Update(float deltaTime) {
	timer::callUpdate(deltaTime);

	character->Update(deltaTime);
	camera->Update(window, deltaTime);

	collision::testMouse(Input::getMousePos());
	Cursor::calcMouseImg();

	if (world::currWorld) {
		for (int i = 0; i < world::currWorld->autoFisherList.size(); i++)
			world::currWorld->autoFisherList[i]->Update(deltaTime);
		if (fishComboWidget)
			fishComboWidget->Update(deltaTime);
		if (world::currWorld->fishTransporter)
			world::currWorld->fishTransporter->update(deltaTime);
		if (pet)
			pet->update(deltaTime);
	}
}

void Main::updateShaders(float deltaTime) {
	// set water movement
	waterShader->Activate();
	waveFactor += waveSpeed * deltaTime;
	tideFactor += deltaTime;
	if (waveFactor >= 1.f) // loop if hit 1
		waveFactor -= 1.f;
	if (tideFactor >= 3.f)
		tideFactor -= 3.f;
	waterShader->setFloat("moveFactor", waveFactor);

	twoDShader->Activate();
	twoDShader->setFloat("tideFactor", tideFactor);

	twoDWaterShader->Activate();
	twoDWaterShader->setFloat("moveFactor", waveFactor);

	// ==== RENDER WATER IMG LAYER ====
	twoDShader->Activate();
	twoDShader->setMat4("projection", camera->getProjectionMat());

	glm::vec2 newPos = math::convertToRelativeCoords(camera->Position);
	glUniform2f(glGetUniformLocation(twoDShader->ID, "playerPos"), newPos.x * 10, newPos.y * 5);

	twoDWaterShader->Activate();
	twoDWaterShader->setMat4("projection", camera->getProjectionMat());
	twoDWaterShader->setVec2("playerPos", glm::vec2(newPos.x * 10, newPos.y * 5));
}

void Main::setupWidgets() {
	pauseMenu = new UpauseMenu(nullptr);
	settingsWidget = new Usettings(nullptr);

	fishComboWidget = new UfishComboWidget(nullptr);

	heldFishWidget = new UheldFishWidget(nullptr);
	heldFishWidget->updateList();
	currencyWidget = new UcurrencyWidget(nullptr);
	currencyWidget->updateList();

	comboWidget = new UcomboWidget(nullptr);
	achievementWidget = new UachievementWidget(nullptr);
	journal = new Ujournal(nullptr);
	fishUnlocked = new UfishUnlocked(nullptr);

	UIWidget = new UUIWidget(nullptr);

	idleProfitWidget = new UidleProfitWidget(nullptr);

	comboOvertimeWiget = new UcomboOvertimeWidget(nullptr);

	newRecordWidget = new UnewRecordWidget(nullptr);
}

void Main::draw3D(Shader* shaderProgram) {
	house->Draw(shaderProgram, *camera);
}

void Main::draw(Shader* shaderProgram) {
	shaderProgram->Activate();
	
	Scene::draw(shaderProgram);

	if (widget::getCurrWidget())
		widget::getCurrWidget()->draw(shaderProgram);

	fishUnlocked->draw(shaderProgram);

	// draw collision
	//collision::showCollisionBoxes(shaderProgram);
}

void Main::windowSizeCallback(GLFWwindow* window, int width, int height) {
	stuff::screenSize = { float(width), float(height) };
	glViewport(0, 0, width, height);
}

void Main::checkInputs() {
	if (Input::getKeyDown(GLFW_KEY_ESCAPE) && Scene::getCurrWorldName() != "titleScreen") {
		//glfwSetWindowShouldClose(window, true);
		if (widget::getCurrWidget()) {
			if (widget::getCurrWidget()->getParent())
				widget::getCurrWidget()->getParent()->addToViewport(true);
			else
				widget::getCurrWidget()->removeFromViewport();
		} else
			pauseMenu->addToViewport(true);
	}

	// temp
	if (Input::getKeyDown(GLFW_KEY_K))
		SaveData::save();
	// temp
	if (Input::getKeyDown(GLFW_KEY_J)) {
		SaveData::saveData.currencyList[1].numOwned += 9000;
		SaveData::saveData.currencyList[1].totalNumOwned += 9000;
		//SaveData::saveData.currencyList[2].numOwned += 150;

		for (int i = 0; i < 0; i++) {
			SaveData::saveData.currencyList[i + 1].numOwned += 1000;
			SaveData::saveData.currencyList[i + 1].totalNumOwned += 1000;
			SaveData::saveData.currencyList[i + 1].unlocked = true;
		}
		currencyWidget->updateList();
	}

	std::string currWorldName = Scene::getCurrWorldName();
	if (Input::getKeyDown(GLFW_KEY_C) && currWorldName != "titleScreen")
		achievementWidget->addToViewport(true);
	if (Input::getKeyDown(GLFW_KEY_V) && currWorldName != "titleScreen")
		journal->addToViewport(true);

	if (Input::getKeyDown(GLFW_KEY_O)) // temp
		Scene::openLevel("rebirth", worldLoc::changeWorlds, false);
}

void Main::drawWidgets(Shader* shaderProgram) {
	if (widget::getCurrWidget() == pauseMenu)
		return;

	fishComboWidget->draw(shaderProgram, stuff::screenSize.x, stuff::screenSize.y);
	heldFishWidget->draw(shaderProgram);
	currencyWidget->draw(shaderProgram);
	comboWidget->draw(shaderProgram);
	UIWidget->draw(shaderProgram);
	comboOvertimeWiget->draw(shaderProgram);
	newRecordWidget->draw(shaderProgram);
}

void Main::rebirth() {
	// resest all progress
	// gives rebirth currency and total rebirth currency
	double addedRebirth = calcRebirthCurrency(); // temp
	SaveData::saveData.rebirthCurrency += addedRebirth;
	SaveData::saveData.totalRebirthCurrency += addedRebirth;

	// reset stuff
	vector playerLoc = { 200, -84 };

	//SaveData::saveData.fishData 
	for (int i = 0; i < SaveData::saveData.fishData.size(); i++) {
		FsaveFishData* currFish = &SaveData::saveData.fishData[i];
		currFish->numOwned = std::vector<double>(4);
	}

	SaveData::saveData.currencyList = std::vector<FsaveCurrencyStruct>(0);
	SaveData::saveData.upgradeList = std::vector<FsaveUpgradeStruct>(0);
	SaveData::saveData.worldList = std::vector<FsaveWorldStruct>(0);
	SaveData::saveData.mechanicStruct = std::vector<FsaveMechanicStruct>(0);
	SaveData::saveData.autoFisherList = std::vector<FsaveAutoFisherStruct>(0);
	SaveData::saveData.petList = std::vector<FsavePetStruct>(0);
	SaveData::saveData.equippedPet.id = -1;
	SaveData::saveData.fishingRod = FsaveFishingRodStruct();
	SaveData::saveData.baitList = std::vector<FsaveBaitStruct>(0);
	SaveData::saveData.buffList = std::vector<FsaveBuffStruct>(0);

	pet = NULL;

	SaveData::recalcLists();

	if (widget::getCurrWidget())
		widget::getCurrWidget()->removeFromViewport();
	if (world::currWorld)
		world::currWorld->autoFisherList.clear();
	heldFishWidget->updateList();
	currencyWidget->updateList();
	Scene::openLevel("world1", 1, true);
}

double Main::calcRebirthCurrency() {
	double totalPoints = 0; // total amount of points, calculated based on fish
	for (int i = 0; i < SaveData::saveData.currencyList.size(); i++) {
		FsaveCurrencyStruct* curr = &SaveData::saveData.currencyList[i];
		totalPoints += curr->totalNumOwned * curr->id;
	}

	double n = pow(SaveData::saveData.totalRebirthCurrency, double(3)) * double(10);			// how many points needed for each level (level 1 = 10, level 2 = 80...)
	double tempRebirthCurrency = std::cbrt((totalPoints + n) / double(10));						// returns level with input of currency
	double pointsToNextRebirth = pow(floor(tempRebirthCurrency + double(1)), double(3)) * double(10); // points needed to get to next level
	double lastRebirthNum = pow(floor(tempRebirthCurrency), double(3)) * double(10);		// how many points the last level took so pretty much y = f(x-1)

	double progress = (totalPoints + n - lastRebirthNum) / (pointsToNextRebirth - lastRebirthNum);
	UIWidget->updateProgressBar(tempRebirthCurrency - SaveData::saveData.totalRebirthCurrency, progress, 0);

	return floor(tempRebirthCurrency - SaveData::saveData.totalRebirthCurrency);
}

void Main::loadIdleProfits() {
	float timeDiffNano = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - SaveData::lastPlayed).count();
	// in seconds
	float timeDiff = timeDiffNano / 1000000000;

	if (world::currWorld && world::currWorld->fishTransporter)
		world::currWorld->fishTransporter->calcIdleProfits(timeDiff);
	if (world::currWorld && world::currWorld->atm)
		world::currWorld->atm->calcIdleProfits(timeDiff);
}