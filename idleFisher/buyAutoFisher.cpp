#include "buyAutoFisher.h"

#include "main.h"
#include "worlds.h"
#include "shortNumbers.h"
#include "character.h"
#include "AautoFisher.h"
#include "animation.h"
#include "collision.h"
#include "fishTransporter.h"

// widget
#include "currencyWidget.h"
#include "text.h"

buyAutoFisher::buyAutoFisher(vector loc) {
	std::unordered_map<std::string, animDataStruct> plusData;
	plusData.insert({ "normal", {{0, 0}, {7, 0}, .1, true } });
	plusData.insert({ "hover", {{0, 1}, {7, 1}, .1, true } });
	plusAnim = std::make_unique<animation>("autoFisher/buyer/buyer.png", 26, 46, plusData, true);
	plusAnim->setAnimation("normal");
	plusAnim->start();

	price = calcPrice();
	priceText = std::make_unique<text>(shortNumbers::convert2Short(price), "bold", loc, true, true);

	updateLoc();
	setupCollision();
}

buyAutoFisher::~buyAutoFisher() {
	// deletes collision from list
	collision::removeCollisionObject(collision.get());
}

void buyAutoFisher::draw(Shader* shaderProgram) {
	if (calcMaxAutoFishers() || !plusAnim)
		return;

	bool prevMouseOver = bMouseOver;
	bMouseOver = plusAnim->spriteSheet->isMouseOver();
	if (!prevMouseOver && bMouseOver) {
		plusAnim->setAnimation("hover");
		Main::hoverObject(NULL);
	} else if (prevMouseOver && !bMouseOver) {
		plusAnim->setAnimation("normal");
		Main::leaveHoverObject(NULL);
	}

	if (bMouseOver && Main::bLeftClick && !Main::currWidget)
		Main::addLeftClick(this, &buyAutoFisher::spawnAutoFisher);

	calcIfPlayerInFront();
	plusAnim->draw(shaderProgram);
	priceText->draw(shaderProgram);
}

void buyAutoFisher::spawnAutoFisher() {
	if (calcMaxAutoFishers())
		return;

	if (!hasCurrency())
		return;


	price = calcPrice();
	SaveData::saveData.currencyList[1].numOwned -= price;
	Main::currencyWidget->updateList();

	//vector loc = Main::autoFisherLocs[(int)Main::autoFisherList.size()];
	std::unique_ptr<AautoFisher> autoFisher = std::make_unique<AautoFisher>(calcAutoFisherId()); // int(world::autoFisherList.size())
	world::currWorld->autoFisherList.push_back(std::move(autoFisher));

	// need to update price after its leveled up
	priceText->setText(shortNumbers::convert2Short(calcPrice()));
	updateLoc();

	if (world::currWorld->fishTransporter)
		world::currWorld->fishTransporter->startPathFinding();

	if (calcMaxAutoFishers()) {
		std::cout << "max autofishers" << std::endl;
		collision::removeCollisionObject(collision.get());
	}
}

int buyAutoFisher::calcAutoFisherId() {
	int autoFisherNumPerWorld = 0; // temp
	int autoFisherWorldNum = 0;
	for (FautoFisherStruct autoFisher : SaveData::data.autoFisherData) {
		if (autoFisher.worldName == "world1")
			autoFisherNumPerWorld++;

		if (autoFisher.worldName == Main::currWorldName && SaveData::saveData.autoFisherList[autoFisher.id].unlocked)
			autoFisherWorldNum++;
	}

	std::string stringNum = Main::currWorldName.substr(std::string("world").size(), Main::currWorldName.size());
	int worldMultiplier = (std::stoi(stringNum) - 1) * autoFisherNumPerWorld + autoFisherWorldNum;

	return worldMultiplier;
}

double buyAutoFisher::calcPrice() {
	// 10 * 1.6^x
	return 10 * pow(1.6, (int)world::currWorld->autoFisherList.size());
}

bool buyAutoFisher::hasCurrency() {
	// temp
	if (SaveData::saveData.currencyList[1].numOwned >= calcPrice())
		return true;
	return false;
}

void buyAutoFisher::updateLoc() {
	if (calcMaxAutoFishers()) {
		Main::leaveHoverObject(NULL);
		return;
	}

	if (plusAnim) {
		plusAnim->setLoc({ SaveData::data.autoFisherData[int(world::currWorld->autoFisherList.size())].xLoc + 4, SaveData::data.autoFisherData[int(world::currWorld->autoFisherList.size())].yLoc + 2 });
		priceText->setLoc(plusAnim->getLoc() + vector{ 0, plusAnim->cellHeight - 3.f * stuff::pixelSize });
	}


	price = calcPrice();
	priceText->setText(shortNumbers::convert2Short(price));

	setupCollision();
}

void buyAutoFisher::calcIfPlayerInFront() {
	vector loc = plusAnim->getLoc();
	inFrontPlayer = Acharacter::getCharLoc().y >= loc.y + 6.f;
}

void buyAutoFisher::setupCollision() {
	if (calcMaxAutoFishers() || !plusAnim)
		return;

	// converts SDL_Point to vector
	std::vector<vector> points = std::vector<vector>(4);
	for (int i = 0; i < 4; i++)
		points[i] = { collisionPoints[i].x + plusAnim->getLoc().x, collisionPoints[i].y + plusAnim->getLoc().y};

	if (!collision) {
		collision = std::make_unique<Fcollision>(points, " ");
		collision::allCollision.push_back(collision.get());
	} else {
		// find collision in list, then replace it
		// reseting it this way cause i need the min and max values to update along with the points
		auto it = std::find(collision::allCollision.begin(), collision::allCollision.end(), collision.get());
		if (it != collision::allCollision.end()) {
			int index = it - collision::allCollision.begin();
			collision = std::make_unique<Fcollision>(points, " ");
			collision::allCollision[index] = collision.get();
		}
	}
}

bool buyAutoFisher::calcMaxAutoFishers() {
	int maxAutoFishers = 0;
	for (FautoFisherStruct autoFisher : SaveData::data.autoFisherData)
		if (autoFisher.worldName == Main::currWorldName)
			maxAutoFishers++;

	return world::currWorld->autoFisherList.size()  >= maxAutoFishers;
}