#pragma once

#include "math.h"

class animation;

class Atree {
public:
	Atree(vector loc, bool isTree);
	~Atree();
	void draw(SDL_Renderer* renderer);

	vector loc;
private:
	animation* treeAnim;
	animation* leafAnim;

	bool isTree = false;

	static inline Fimg* treeSpriteSheet;
	static inline Fimg* leafSpriteSheet;
	static inline Fimg* bushSpriteSheet;
};