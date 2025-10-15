#include "node.h"

node::node(bool _walkable, vector _worldPos, int _gridX, int _gridY) {
	walkable = _walkable;
	loc = _worldPos;
	gridX = _gridX;
	gridY = _gridY;
}

int node::fCost() {
	return gCost + hCost;
}