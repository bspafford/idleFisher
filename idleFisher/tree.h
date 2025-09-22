#pragma once

#include "math.h"

class animation;
class Shader;

class Atree {
public:
	Atree(vector loc, bool isTree);
	~Atree();
	void draw(Shader* shaderProgram);

	vector loc;
private:
	animation* treeAnim;
	animation* leafAnim;

	bool isTree = false;

	static inline Image* treeSpriteSheet;
	static inline Image* leafSpriteSheet;
	static inline Image* bushSpriteSheet;

	Image* test;
};