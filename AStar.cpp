#include "AStar.h"
#include "main.h"
#include "collision.h"
#include "pathfinding.h"
#include "path.h"
#include "saveData.h"

AStar::AStar() {
	stopPathFinding();
}

void AStar::init() {
	gridWorldSize = { 350, 175 };
	nodeSize = 8;

	gridSize.x = round(gridWorldSize.x / nodeSize * stuff::pixelSize);
	gridSize.y = round(gridWorldSize.y / nodeSize * stuff::pixelSize);

	gridOffset = { -100, -375 };
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
			for (int i = 0; i < Main::allCollision.size(); i++) {
				if (collision::isCloseEnough(col, Main::allCollision[i])) {
					vector normal;
					float depth;
					if (collision::intersectPolygons(temp, Main::allCollision[i]->points, normal, depth)) {
						walkable = false;
					}
				}
			}

			grid[y][x] = new node(walkable, loc, x, y);
		}
	}
}

void AStar::drawBoard(SDL_Renderer* renderer) {
	if (path)
		path->draw(renderer);
	//return;

	// draw grid
	for (int y = 0; y < gridSize.y; y++) {
		for (int x = 0; x < gridSize.x; x++) {
			vector loc = math::worldToScreen(grid[y][x]->loc);
			if (grid[y][x]->parent)
				SDL_SetRenderDrawColor(renderer, 255, 0, 255, 50);
			else if (grid[y][x]->walkable)
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 50);
			else
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
			SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) };
			SDL_RenderFillRect(renderer, rect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
			SDL_RenderDrawRect(renderer, rect);
		}
	}

	// draw path
	// std::cout << "path size: " << path.size() << std::endl;
	for (int i = 0; i < nodePath.size(); i++) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		vector loc = math::worldToScreen(nodePath[i].loc);
		SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) };
		SDL_RenderFillRect(renderer, rect);
	}

	// temp draw start and end
	node startNode = *AStar::nodeFromWorldPoint(startPos);
	node targetNode = *AStar::nodeFromWorldPoint(targetPos);
	vector loc = math::worldToScreen(startNode.loc);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
	SDL_RenderFillRect(renderer, new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) });
	loc = math::worldToScreen(targetNode.loc);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
	SDL_RenderFillRect(renderer, new SDL_Rect{ int(loc.x), int(loc.y), int(nodeSize * stuff::pixelSize), int(nodeSize * stuff::pixelSize) });
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

vector AStar::followPath(vector loc, float deltaTime) {
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
	} else if (math::distance(targetPos, pos) > 1) {
		// go to end point
		vector diff = math::normalize(targetPos - pos) * deltaTime;
		return diff;
	} else {
		followingPath = false;
		finished = true;
		return { 0, 0 };
	}

}