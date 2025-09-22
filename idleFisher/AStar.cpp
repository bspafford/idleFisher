#include "AStar.h"
#include "main.h"
#include "collision.h"
#include "pathfinding.h"
#include "path.h"
#include "saveData.h"
#include "Rectangle.h"

AStar::AStar() {
	stopPathFinding();
}

void AStar::init() {
	gridWorldSize = { 350, 175 };
	nodeSize = 8;

	gridSize.x = round(gridWorldSize.x / nodeSize * stuff::pixelSize);
	gridSize.y = round(gridWorldSize.y / nodeSize * stuff::pixelSize);

	gridOffset = { 400, 350 };// { -100, -375 };
	// create grid
	grid = new node * *[gridSize.y];
	for (int y = 0; y < gridSize.y; y++) {
		grid[y] = new node * [gridSize.x];
		for (int x = 0; x < gridSize.x; x++) {
			// do col test
			vector loc = vector{ nodeSize * x, nodeSize * y } + gridOffset;

			std::vector<vector> temp = std::vector<vector>(4);
			temp[0] = loc;
			temp[1] = loc + vector{ nodeSize, 0 };
			temp[2] = loc + vector{ nodeSize, nodeSize };
			temp[3] = loc + vector{ 0, nodeSize };
			Fcollision* col = new Fcollision(temp, "");

			bool walkable = true;
			for (int i = 0; i < collision::allCollision.size(); i++) {
				if (collision::isCloseEnough(col, collision::allCollision[i])) {
					vector normal;
					float depth;
					if (collision::intersectPolygons(temp, collision::allCollision[i]->points, normal, depth)) {
						walkable = false;
					}
				}
			}

			grid[y][x] = new node(walkable, loc, x, y);
		}
	}
}

void AStar::drawBoard(Shader* shaderProgram) {
	if (path)
		path->draw(shaderProgram);

	/*
	glm::vec4 color = glm::vec4(255, 0, 255, 50);
	URectangle rect(gridOffset, gridWorldSize * stuff::pixelSize * stuff::pixelSize, true, color);
	rect.draw(shaderProgram);
	return;
	*/

	// draw grid
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			vector loc = grid[y][x]->loc;// math::worldToScreen(grid[y][x]->loc, "topleft");
			glm::vec4 color;
			if (grid[y][x]->parent)
				color = glm::vec4(255, 0, 255, 50);
			else if (grid[y][x]->walkable)
				color = glm::vec4(0, 255, 255, 50);
			else {
				color = glm::vec4(255, 0, 0, 50);
				URectangle rect(loc, vector{ 1, 1 } *nodeSize * stuff::pixelSize, true, color);
				rect.draw(shaderProgram);
			}
			// draw outline
			//SDL_SetRenderDrawColor(shaderProgram, 0, 0, 0, 50);
			//SDL_RenderDrawRect(shaderProgram, rect);
		}
	}

	URectangle rect(targetPos, vector{ 3, 3 }, true);
	rect.draw(shaderProgram);

	/*
	// draw path
	// std::cout << "path size: " << path.size() << std::endl;
	for (int i = 0; i < nodePath.size(); i++) {
		SDL_SetRenderDrawColor(shaderProgram, 0, 0, 255, 255);
		vector loc = math::worldToScreen(nodePath[i].loc);
		SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) };
		SDL_RenderFillRect(shaderProgram, rect);
	}

	// temp draw start and end
	node startNode = *AStar::nodeFromWorldPoint(startPos);
	node targetNode = *AStar::nodeFromWorldPoint(targetPos);
	vector loc = math::worldToScreen(startNode.loc);
	SDL_SetRenderDrawColor(shaderProgram, 0, 255, 0, 100);
	SDL_RenderFillRect(shaderProgram, new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) });
	loc = math::worldToScreen(targetNode.loc);
	SDL_SetRenderDrawColor(shaderProgram, 255, 255, 0, 100);
	SDL_RenderFillRect(shaderProgram, new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) });
	*/
}

node* AStar::nodeFromWorldPoint(vector worldPos) {
	float percentX = (worldPos.x - gridOffset.x) / gridWorldSize.x / stuff::pixelSize;
	float percentY = (worldPos.y - gridOffset.y) / gridWorldSize.y / stuff::pixelSize;
	percentX = math::clamp(percentX, 0, 1);
	percentY = math::clamp(percentY, 0, 1);

	int x = round((gridSize.x - 1) * percentX);
	int y = round((gridSize.y - 1) * percentY);
	return grid[y][x];
}

std::vector<node*> AStar::getNeighbors(node* currNode) {
	std::vector<node*> neighbors;

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (x == 0 && y == 0)
				continue;

			int checkX = currNode->gridX + x;
			int checkY = currNode->gridY + y;

			if (checkX >= 0 && checkX < gridSize.x && checkY >= 0 && checkY < gridSize.y) {
				neighbors.push_back(grid[checkY][checkX]);
			}
		}
	}

	return neighbors;
}

void AStar::startPathFinding(vector startLoc, vector targetLoc) {
	stopped = false;
	pathIndex = 0;
	finished = false;

	startPos = startLoc;
	targetPos = targetLoc;
	pathfinding::findPath(startPos, targetPos, this);
}

void AStar::stopPathFinding() {
	pathIndex = 0;
	stopped = true;
}

void AStar::onPathFound(std::vector<vector> waypoints, bool pathSuccessful) {
	//vector startPos = { 0, 0 }; // temp

	if (pathSuccessful) {
		path = new Path(waypoints, startPos, turnDst);
		// start followPath()
		followingPath = true;
	}
}

vector AStar::followPath(vector loc, float deltaTime, float speed) {
	if (!followingPath || stopped)
		return { 0, 0 };

	vector pos = loc;
	if (pathIndex != path->finishLineIndex && path->turnBoundaries.size() > 0) {
		while (path->turnBoundaries[pathIndex]->hasCrossedLine(pos)) {
			if (pathIndex == path->finishLineIndex) {
				//followingPath = false;
				break;
			} else
				pathIndex++;
		}
		
		if (followingPath) {
			vector diff = math::normalize(path->lookPoints[pathIndex] - pos) * deltaTime;
			return diff;
		}
		// distance from current pos to end,		how far the character moves each frame
	} else if (math::distance(targetPos, pos) >= math::length(math::normalize(targetPos - pos) * deltaTime * speed)) { // if outside of target position range
		std::cout << "going to target pos!\n";
		// go to end point
		vector diff = math::normalize(targetPos - pos) * deltaTime;
		return diff;
	} else { // if at target position
		std::cout << "at target pos!\n";
		followingPath = false;
		finished = true;

		// return diff to put character at target pos
		return (targetPos - pos) / speed;
	}

}