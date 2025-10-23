#pragma once

#include <set>
#include <cassert>
#include <functional>

#include "math.h"
#include "saveData.h"
#include "Hoverable.h"
#include "NPCwidget.h"
#include "animation.h"
#include "collision.h"

class Shader;
class Image;

class npc : public IHoverable {
protected:
	npc(vector loc);
public:
	virtual ~npc();

	static inline std::vector<npc*> instances;

	virtual void draw(Shader* shaderProgram);

	virtual void setLoc(vector loc);

	// returns true if player is infront of the NPC
	virtual bool calcIfPlayerInfront();

	std::weak_ptr<Image> getCharImg();

	virtual void click();

	bool isDiscovered();
protected:
	void setup(std::string npcName);
	virtual void setupCollision();

	vector loc;

	bool bMouseOver = false;

	std::vector<vector> collisionPoints;

	std::unique_ptr<animation> npcAnim;

	std::unique_ptr<NPCwidget> widget;

	std::unique_ptr<animation> exclamationPointAnim;

	bool* discovered = nullptr;
	// when there is no saveData value to put for discovered, will fall back to pointing to this value
	bool discoveredFallback = true;
private:
	// gets the offset for collision based on charImg;
	vector getOffset();

	std::unique_ptr<Fcollision> col;
};