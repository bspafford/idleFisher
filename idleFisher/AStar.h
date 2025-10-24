#pragma once

#include "math.h"
#include "node.h"

class Path;
class Shader;

class AStar {
public:
	AStar();
	static void Deconstructor();
	// sets up the board
	static void init();
	void drawBoard(Shader* shaderProgram);
	node* nodeFromWorldPoint(vector worldPos);
	std::vector<node*> getNeighbors(node& currNode);
	// returns the move diff
	void startPathFinding(vector startLoc, vector targetLoc);
	void stopPathFinding();

	void onPathFound(std::vector<vector> waypoints, bool pathSuccessful);
	vector followPath(vector loc, float deltaTime, float speed);
	std::unique_ptr<Path> path;
	float turnDst = 20;
	bool followingPath = false;
	bool finished = false;
	bool stopped = true;

	int getIndex(int x, int y);

private:
	static inline std::vector<node> grid;
	static inline vector gridOffset;
	static inline vector gridWorldSize;
	static inline vector gridSize;
	static inline float nodeSize; // square

	int pathIndex;

	vector startPos;
	vector targetPos;
public:
	std::vector<node> nodePath;
};