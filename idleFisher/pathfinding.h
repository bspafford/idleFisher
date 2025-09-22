#pragma once

#include "math.h"

class node;
class AStar;

class pathfinding {
public:
	pathfinding();
	static std::vector<vector> retracePath(node* startNode, node* endNode, AStar* AStarRef);
	static int getDistance(node* a, node* b);

	static void findPath(vector startPos, vector targetPos, AStar* AStarRef);

	static void eraseFromList(std::vector<node*> &list, node* node);

	static std::vector<vector> simplifyPath(std::vector<node> path);
};