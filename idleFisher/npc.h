#pragma once

#include <set>
#include <cassert>
#include <functional>

#include "math.h"
#include "saveData.h"
#include "NPCwidget.h"
#include "animation.h"

class Shader;
class Image;

class npc {
protected:
	npc(vector loc);
public:
	virtual ~npc();

	static inline std::set<npc*> instances;

	virtual void draw(Shader* shaderProgram);

	virtual void setLoc(vector loc);

	// returns true if player is infront of the NPC
	bool calcIfPlayerInfront();

	Image* getCharImg();

	void removeDanglingWidget();

	virtual void click();

	bool isDiscovered();
protected:
	void setup(std::string npcName);
	virtual void setupCollision();

	bool mouseOver(Image* img);
	virtual void mouseEnter();
	virtual void mouseExit();
	
	vector loc;

	bool bMouseOver = false;

	std::vector<vector> collisionPoints;

	animation* npcAnim;

	NPCwidget* widget;

	animation* exclamationPointAnim;

	bool* discovered;
private:
	// gets the offset for collision based on charImg;
	vector getOffset();
};