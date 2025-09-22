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
	plusAnim = new animation("autoFisher/buyer/buyer.png", 26, 46, plusData, true);
	plusAnim->setAnimation("normal");
	plusAnim->start();

	price = calcPrice();
	priceText = new text(shortNumbers::convert2Short(price), "bold", loc, true, true);

	updateLoc();
	setupCollision();
}

void buyAutoFisher::draw(Shader* shaderProgram) {
	if (calcMaxAutoFishers() || !plusAnim)
		return;

	bool prevMouseOver = bMouseOver;
	bMouseOver = mouseOver();
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
	AautoFisher* autoFisher = new AautoFisher(calcAutoFisherId()); // int(world::autoFisherList.size())
	world::autoFisherList.push_back(autoFisher);

	// need to update price after its leveled up
	priceText->setText(shortNumbers::convert2Short(calcPrice()));
	updateLoc();

	if (world::fishTransporter)
		world::fishTransporter->startPathFinding();

	if (calcMaxAutoFishers()) {
		std::cout << "max autofishers" << std::endl;
		auto it = std::find(collision::allCollision.begin(), collision::allCollision.end(), collision);
		if (it != collision::allCollision.end())
			collision::allCollision.erase(it);
	}
}

int buyAutoFisher::calcAutoFisherId() {
	int autoFisherNumPerWorld = 0; // temp
	int autoFisherWorldNum = 0;
	for (FautoFisherStruct autoFisher : SaveData::data.autoFisherData) {
		if (autoFisher.worldName == "world1")
			autoFisherNumPerWorld++;

		if (autoFisher.worldName == Main::currWorld && SaveData::saveData.autoFisherList[autoFisher.id].unlocked)
			autoFisherWorldNum++;
	}

	std::string stringNum = Main::currWorld.substr(std::string("world").size(), Main::currWorld.size());
	int worldMultiplier = (std::stoi(stringNum) - 1) * autoFisherNumPerWorld + autoFisherWorldNum;

	return worldMultiplier;
}

bool buyAutoFisher::mouseOver() {
	if (!plusAnim)
		return false;

	vector mousePos = Main::mousePos;
	vector screenLoc = math::worldToScreen(plusAnim->getLoc(), "topleft");
	if (mousePos.x >= screenLoc.x && mousePos.x <= screenLoc.x + plusAnim->cellWidth * stuff::pixelSize && mousePos.y >= screenLoc.y - plusAnim->cellHeight * stuff::pixelSize && mousePos.y <= screenLoc.y)
		return true;
	return false;
}

double buyAutoFisher::calcPrice() {
	// 10 * 1.6^x
	return 10 * pow(1.6, (int)world::autoFisherList.size());
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
		plusAnim->setLoc({ SaveData::data.autoFisherData[int(world::autoFisherList.size())].xLoc + 4, SaveData::data.autoFisherData[int(world::autoFisherList.size())].yLoc + 2 });
		priceText->setLoc(plusAnim->getLoc() + vector{ 0, plusAnim->cellHeight - 3.f * stuff::pixelSize });
	}


	price = calcPrice();
	priceText->setText(shortNumbers::convert2Short(price));

	setupCollision();
}

void buyAutoFisher::calcIfPlayerInFront() {
	// should test if they are close enough to eachother?
	vector imgLoc = Main::character->anim->getLoc();
	vector imgSize = { Main::character->anim->cellWidth, Main::character->anim->cellHeight };
	//vector playerLoc = math::screenToWorld(imgLoc.x + imgSize.x / 2 * stuff::pixelSize, imgLoc.y + imgSize.y * stuff::pixelSize);
	vector playerLoc = Main::character->getCharLoc();
	vector screenLoc = math::worldToScreen(playerLoc, "topleft");
	vector screenAutoLoc = math::worldToScreen(plusAnim ? plusAnim->getLoc() : vector{0, 0}, "topleft");
	screenAutoLoc = { screenAutoLoc.x, screenAutoLoc.y + 39 * stuff::pixelSize };

	inFrontPlayer = !(screenLoc.y > screenAutoLoc.y);
}

void buyAutoFisher::setupCollision() {
	if (calcMaxAutoFishers() || !plusAnim)
		return;

	// converts SDL_Point to vector
	std::vector<vector> points = std::vector<vector>(4);
	for (int i = 0; i < 4; i++)
		points[i] = { collisionPoints[i].x + plusAnim->getLoc().x, collisionPoints[i].y + plusAnim->getLoc().y};

	if (!collision) {
		collision = new Fcollision(points, " ");
		collision::allCollision.push_back(collision);
	} else {
		// find collision in list, then replace it
		// reseting it this way cause i need the min and max values to update along with the points
		auto it = std::find(collision::allCollision.begin(), collision::allCollision.end(), collision);
		if (it != collision::allCollision.end()) {
			int index = it - collision::allCollision.begin();
			delete collision::allCollision[index];
			collision = new Fcollision(points, " ");
			collision::allCollision[index] = collision;
		}
	}
}

bool buyAutoFisher::calcMaxAutoFishers() {
	int maxAutoFishers = 0;
	for (FautoFisherStruct autoFisher : SaveData::data.autoFisherData)
		if (autoFisher.worldName == Main::currWorld)
			maxAutoFishers++;

	return world::autoFisherList.size()  >= maxAutoFishers;
}