#pragma once

#include "math.h"
#include "node.h"

class Path;
class Shader;

class AStar {
public:
	AStar();
	// sets up the board
	static void init();
	void drawBoard(Shader* shaderProgram);
	node* nodeFromWorldPoint(vector worldPos);
	std::vector<node*> getNeighbors(node* currNode);
	// returns the move diff
	void startPathFinding(vector startLoc, vector targetLoc);
	void stopPathFinding();

	// asdf
	void onPathFound(std::vector<vector> waypoints, bool pathSuccessful);
	vector followPath(vector loc, float deltaTime, float speed);
	Path* path;
	float turnDst = 20;
	bool followingPath = false;
	bool finished = false;
	bool stopped = true;

private:
	static inline node*** grid;
	static inline vector gridOffset;
	static inline vector gridWorldSize;
	static inline vector gridSize;
	static inline float nodeSize; // square

	int pathIndex;

	// temp
	vector startPos;
	vector targetPos;
public:
	std::vector<node> nodePath;
};