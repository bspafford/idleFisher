#pragma once

#include "math.h"

class node {
public:
	node(bool _walkable, vector _worldPos, int _gridX, int _gridY);
	int fCost();

	vector loc;
	bool walkable;
	int gridX;
	int gridY;

	int gCost;
	int hCost;
	node* parent;
};