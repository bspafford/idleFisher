#include "tree.h"

#include "animation.h"

Atree::Atree(vector loc, bool isTree) {
	this->isTree = isTree;
	this->loc = loc;

	if (isTree) {
		if (!treeSpriteSheet)
			treeSpriteSheet = new Fimg("./images/landscape/tree.png");
		if (!leafSpriteSheet)
			leafSpriteSheet = new Fimg("./images/landscape/leafParticles.png");

		std::unordered_map<std::string, animDataStruct> treeData;
		treeData.insert({ "tree", {{0, 0}, {29, 0}, .1, true} });
		loc = loc - vector{ 94 / 2, 122 };
		treeAnim = new animation(treeSpriteSheet, 94, 122, treeData, true, loc);
		treeAnim->setAnimation("tree");
		treeAnim->start();

		std::unordered_map<std::string, animDataStruct> leafData;
		leafData.insert({ "leaf", {{0, 0}, {42, 0}, .1, true} });
		leafAnim = new animation(leafSpriteSheet, 128, 69, leafData, true, loc + vector{ 24, 47 });
		leafAnim->setAnimation("leaf");
		leafAnim->start();

		// set to random frame
		treeAnim->currFrameLoc.x = round(math::randRange(0, 29));
		leafAnim->currFrameLoc.x = round(math::randRange(0, 42));
	} else {
		if (!bushSpriteSheet)
			bushSpriteSheet = new Fimg("./images/landscape/bush.png");

		std::unordered_map<std::string, animDataStruct> bushData;
		bushData.insert({ "bush", {{0, 0}, {29, 0}, .1, true} });
		loc = loc - vector{ 37 / 2, 33 };
		treeAnim = new animation(bushSpriteSheet, 37, 33, bushData, true, loc);
		treeAnim->setAnimation("bush");
		treeAnim->start();

		treeAnim->currFrameLoc.x = round(math::randRange(0, 29));
	}
}

Atree::~Atree() {
	delete treeAnim;
	delete leafAnim;
	treeAnim = nullptr;
	leafAnim = nullptr;
}

void Atree::draw(SDL_Renderer* renderer) {
	if (!treeAnim)
		return;

	treeAnim->draw(renderer);
	if (leafAnim && isTree)
		leafAnim->draw(renderer);

	// draw red dot to see bot mid of tree
	//vector screenLoc = math::worldToScreen(loc);
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//SDL_RenderDrawPoint(renderer, screenLoc.x, screenLoc.y);
}