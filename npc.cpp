#include "npc.h"

#include "main.h"
#include "Acharacter.h"
#include "collision.h"
#include "animation.h"

npc::npc(vector loc) {
	instances.insert(this);
	this->loc = loc;

	std::unordered_map<std::string, animDataStruct> exclamationPointAnimData;
	exclamationPointAnimData.insert({ "anim", {{0, 0}, {4, 0}, .1, true} });
	exclamationPointAnim = new animation("widget/npcButtons/exclamationPoint.png", 5, 13, exclamationPointAnimData, true, loc);
	exclamationPointAnim->setAnimation("anim");
	exclamationPointAnim->start();
}

npc::~npc() {
	instances.erase(this);
	delete this->widget;
	delete exclamationPointAnim;
	widget = nullptr;
	exclamationPointAnim = nullptr;
}

void npc::setup(std::string npcName) {
	if (widget)
		delete widget;
	widget = new NPCwidget(this, npcName);

	std::unordered_map<std::string, vector> npcCellSizes;
	npcCellSizes.insert({ "atm", { 28, 47 } });
	npcCellSizes.insert({ "decorator", { 29, 54 } });
	npcCellSizes.insert({ "fisherman", { 34, 54 } });
	npcCellSizes.insert({ "mechanic", { 32, 58 } });
	npcCellSizes.insert({ "merchant", { 19, 49 } });
	npcCellSizes.insert({ "petSeller", { 36, 51 } });
	npcCellSizes.insert({ "sailor", { 21, 49 } });
	npcCellSizes.insert({ "scuba", { 18, 45 } });
	npcCellSizes.insert({ "fishTransporter", {21, 49} });

	std::unordered_map<std::string, animDataStruct> npcAnimData;
	npcAnimData.insert({ "idle", {{0, 0}, {13, 0}, .1, true} });
	vector cellSize = npcCellSizes[npcName];
	npcAnim = new animation("npc/" + npcName + ".png", cellSize.x, cellSize.y, npcAnimData, true, loc);
	npcAnim->spriteSheet->loadSurface("./images/npc/" + npcName + ".png");
	npcAnim->setAnimation("idle");
	npcAnim->start();
	npcAnim->currFrameLoc.x = round(math::randRange(0, 13));

	setLoc(loc);
	setupCollision();
}

void npc::setupCollision() {
	// draw by feet / bottom of img
	vector offset = getOffset();
	float radius = 7;

	Fcollision* col = new Fcollision(offset, radius, " ");
	Main::allCollision.push_back(col);
}

void npc::draw(SDL_Renderer* renderer) {
	mouseOver(npcAnim->spriteSheet);
	npcAnim->draw(renderer);

	if (!isDiscovered())
		exclamationPointAnim->draw(renderer);
}

void npc::setLoc(vector loc) {
	this->loc = loc;
	//charImg->loc = loc;

	npcAnim->setLoc(loc - vector{ npcAnim->cellWidth / 2.f, float(npcAnim->cellHeight) });
	exclamationPointAnim->setLoc(npcAnim->getLoc() + vector{npcAnim->cellWidth / 2.f - exclamationPointAnim->cellWidth / 2, -float(exclamationPointAnim->cellHeight + 2)});
	if (exclamationPointAnim)
		exclamationPointAnim->setLoc(exclamationPointAnim->getLoc());

	// setupCollision();
}

bool npc::mouseOver(Fimg* img) {
	if (Main::currWidget || !img)
		return false;

	vector min = math::worldToScreen(img->loc);
	vector max = min + (vector{ float(img->w), float(img->h) } *stuff::pixelSize);

	if (min.x <= Main::mousePos.x && Main::mousePos.x <= max.x && min.y <= Main::mousePos.y && Main::mousePos.y <= max.y) {
		vector screenPos = min;
		vector pos = { Main::mousePos.x - screenPos.x, Main::mousePos.y - screenPos.y };
		SDL_Color pixelColor = math::GetPixelColor(img->surface, (int)pos.x, (int)pos.y);
		if ((int)pixelColor.a != 0) {
			if (!bMouseOver)
				mouseEnter();

			if (Main::bLeftClick && bMouseOver)
				Main::addLeftClick(this, &npc::click);
				//click();
			return true;
		}
	}

	if (bMouseOver)
		mouseExit();
	return false;
}

void npc::click() {
	// if not clicking over NPC
	//widget = new NPCwidget(this);
	widget->addToViewport(true);
}

bool npc::isDiscovered() {
	if (discovered)
		return *discovered;
	return false;
}

void npc::mouseEnter() {
	bMouseOver = true;

	Main::hoverObject(NULL);
}

void npc::mouseExit() {
	bMouseOver = false;

	Main::leaveHoverObject(NULL);
}

bool npc::calcIfPlayerInfront() {
	vector playerWorldLoc;
	if (Main::character->anim)
		playerWorldLoc =  SaveData::saveData.playerLoc / stuff::pixelSize + Main::character->anim->cellHeight / 2;
	return (playerWorldLoc.y > loc.y);
}

vector npc::getOffset() {
	return loc;
}

Fimg* npc::getCharImg() {
	return npcAnim->spriteSheet;
}

void npc::removeDanglingWidget() {
	widget = nullptr;
}
