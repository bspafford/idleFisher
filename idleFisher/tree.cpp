#include "tree.h"

#include "animation.h"
#include "Image.h"

Atree::Atree(vector loc, bool isTree) {
	this->isTree = isTree;
	this->loc = loc;

	if (isTree) {
		if (!treeSpriteSheet)
			treeSpriteSheet = new Image("./images/landscape/tree.png", { 0, 0 }, true);
		if (!leafSpriteSheet)
			leafSpriteSheet = new Image("./images/landscape/leafParticles.png", { 0, 0 }, true);

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
			bushSpriteSheet = new Image("./images/landscape/bush.png", { 0, 0 }, true);

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

void Atree::draw(Shader* shaderProgram) {
	if (!treeAnim)
		return;

	treeAnim->draw(shaderProgram);
	if (leafAnim && isTree)
		leafAnim->draw(shaderProgram);

	// draw red dot to see bot mid of tree
	//vector screenLoc = math::worldToScreen(loc);
	//SDL_SetRenderDrawColor(shaderProgram, 255, 0, 0, 255);
	//SDL_RenderDrawPoint(shaderProgram, screenLoc.x, screenLoc.y);
}