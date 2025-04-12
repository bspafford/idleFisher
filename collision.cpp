#include "collision.h"

#include <string>
#include <fstream>

#include "main.h"
#include "saveData.h"
#include "Acharacter.h"

collision::collision() {

}

Fcollision::Fcollision(std::vector<vector> worldPoints, std::string identifier) {
	isCircle = false;
	this->points = worldPoints;
	this->identifier = identifier;

	minX = INFINITY, maxX = -INFINITY, minY = INFINITY, maxY = -INFINITY;

	// world loc
	for (int i = 0; i < worldPoints.size(); i++) {
		if (points[i].x < minX)
			minX = points[i].x;
		else if (points[i].x > maxX)
			maxX = points[i].x;

		if (points[i].y < minY)
			minY = points[i].y;
		else if (points[i].y > maxY)
			maxY = points[i].y;
	}
}

Fcollision::Fcollision(vector center, float radius, std::string identifier) {
	isCircle = true;
	this->radius = radius;
	this->identifier = identifier;

	points.push_back(center);

	minX = center.x - radius;
	maxX = center.x + radius;
	minY = center.y - radius;
	maxY = center.y + radius;
}

void collision::getCollisionObjects(std::vector<Fcollision*>& allCollision) {
	// clears all collision
	allCollision.clear();

	std::vector<std::vector<vector>> coords;

	std::ifstream colFile("./data/collision.col");

	int lineNum = 0;
	std::string currWorldName = "";
	if (colFile.is_open()) {
		while (colFile.good()) {
			std::string line;
			std::getline(colFile, line);

			removeSpaces(line);

			if (line[0] == '!' ) {
				if (currWorldName != "") // returns if passed the world
					return;

				// then set the world name parameter
				line.erase(line.begin());
				if (line == Main::currWorld)
					currWorldName = line;

			// only puts collision in the list if its part of that world
			} else if (line[0] != '/' && line != "" && currWorldName != "") {

				std::string identifier = getIdentifier(line);
				
				line.erase(0, identifier.size());

				std::vector<std::string> stringList;
				for (int i = 0; i < line.size(); i++) {
					// if char == "(" then break into a new string
					if (line[i] == '(') {
						std::string temp(1, line[i]);
						stringList.push_back(temp);
					} else {
						std::string temp(1, line[i]);
						stringList[stringList.size() - 1].append(temp);
					}
				}

				if ((int)coords.size() - 1 < lineNum) {
					std::vector<vector> temp(stringList.size());
					coords.push_back(temp);
				}

				for (int i = 0; i < stringList.size(); i++) {
					sscanf_s(stringList[i].c_str(), "(%f, %f)", &coords[lineNum][i].x, &coords[lineNum][i].y);
				}

				Fcollision* col = new Fcollision(coords[lineNum], identifier);
				Main::allCollision.push_back(col);

				lineNum++;
			}
		}
	}
}

bool collision::mouseOverWater(vector mousePos, std::vector<Fcollision*> allCollision) {
	mousePos = math::screenToWorld(mousePos.x, mousePos.y);

	// mousePos
	Fcollision* aCol = new Fcollision(mousePos, 1, "");

	vector normal;
	float depth;
	for (int i = 0; i < allCollision.size(); i++) {
		if (allCollision[i]->identifier == "w" && isCloseEnough(aCol, allCollision[i]) && intersectCirclePolygon(mousePos, 1, allCollision[i]->points, normal, depth))
			return true;
	}

	return false;
}

bool collision::intersectCirclePolygon(vector circleCenter, float circleRadius, std::vector<vector> vertices, vector& normal, float& depth) {
	normal = { 0, 0 };
	depth = INFINITY;

	for (int i = 0; i < vertices.size(); i++) {
		vector va = vertices[i];
		vector vb = vertices[(i + 1) % vertices.size()];

		vector edge = va - vb;
		vector axis = { -edge.y, edge.x };
		axis = math::normalize(axis);

		float minA, maxA;
		float minB, maxB;
		projectVertices(vertices, axis, minA, maxA);
		projectCircle(circleCenter, circleRadius, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA) {
			return false;
		}

		float axisDepth = math::min(maxB - minA, maxA - minB);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}

	int cpIndex = findClosestPointOnPolygon(circleCenter, vertices);
	vector cp = vertices[cpIndex];

	vector axis = cp - circleCenter;
	axis = math::normalize(axis);

	float minA, maxA;
	float minB, maxB;
	projectVertices(vertices, axis, minA, maxA);
	projectCircle(circleCenter, circleRadius, axis, minB, maxB);

	if (minA >= maxB || minB >= maxA) {
		return false;
	}

	float axisDepth = math::min(maxB - minA, maxA - minB);
	if (axisDepth < depth) {
		depth = axisDepth;
		normal = axis;
	}

	vector polygonCenter = findArithmeticMean(vertices);

	vector direction = polygonCenter - circleCenter;

	if (math::dot(direction, normal) < 0) // its the wrong direction
		normal = { -normal.x, -normal.y };

	return true;
}

int collision::findClosestPointOnPolygon(vector circleCenter, std::vector<vector> vertices) {
	int result = -1;
	float minDistance = INFINITY;

	for (int i = 0; i < vertices.size(); i++) {
		vector v = vertices[i];
		float distance = math::distance(v, circleCenter);

		if (distance < minDistance) {
			minDistance = distance;
			result = i;
		}
	}

	return result;
}

void collision::projectCircle(vector center, float radius, vector axis, float& min, float& max) {
	vector direction = math::normalize(axis);
	vector directionAndRadius = direction * radius;

	vector p1 = center + directionAndRadius;
	vector p2 = center - directionAndRadius;

	min = math::dot(p1, axis);
	max = math::dot(p2, axis);

	if (min > max) {
		// swaps the min and max values
		float t = min;
		min = max;
		max = t;
	}
}

bool collision::intersectPolygons(std::vector<vector> verticesA, std::vector<vector> verticesB, vector& normal, float& depth) {
	normal = { 0, 0 };
	depth = INFINITY;

	for (int i = 0; i < verticesA.size(); i++) {
		vector va = verticesA[i];
		vector vb = verticesA[(i + 1) % verticesA.size()];

		vector edge = va - vb;
		vector axis = { -edge.y, edge.x };
		axis = math::normalize(axis);

		float minA, maxA;
		float minB, maxB;
		projectVertices(verticesA, axis, minA, maxA);
		projectVertices(verticesB, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA) {
			return false;
		}

		float axisDepth = math::min(maxB - minA, maxA - minB);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}

	for (int i = 0; i < verticesB.size(); i++) {
		vector va = verticesB[i];
		vector vb = verticesB[(i + 1) % verticesB.size()];

		vector edge = vb - va;
		vector axis = { -edge.y, edge.x };
		axis = math::normalize(axis);

		float minA, maxA;
		float minB, maxB;
		projectVertices(verticesA, axis, minA, maxA);
		projectVertices(verticesB, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA) {
			return false;
		}

		float axisDepth = math::min(maxB - minA, maxA - minB);
		if (axisDepth < depth) {
			depth = axisDepth;
			normal = axis;
		}
	}

	vector centerA = findArithmeticMean(verticesA);
	vector centerB = findArithmeticMean(verticesB);

	vector direction = centerB - centerA;

	if (math::dot(direction, normal) < 0) // its the wrong direction
		normal = { -normal.x, -normal.y };

	return true;
}

void collision::projectVertices(std::vector<vector> vertices, vector axis, float& min, float& max) {
	min = INFINITY;
	max = -INFINITY;

	for (int i = 0; i < vertices.size(); i++) {
		vector v = vertices[i];
		float proj = math::dot(v, axis);

		if (proj < min) { min = proj; }
		if (proj > max) { max = proj; }
	}
}

vector collision::findArithmeticMean(std::vector<vector> vertices) {
	float sumX = 0;
	float sumY = 0;

	for (int i = 0; i < vertices.size(); i++) {
		vector v = vertices[i];
		sumX += v.x;
		sumY += v.y;
	}

	return { sumX / (float)vertices.size(), sumY / (float)vertices.size() };
}

bool collision::intersectCircles(vector centerA, float radiusA, vector centerB, float radiusB, vector& normal, float& depth) {
	normal = { 0, 0 };
	depth = 0;

	float distance = math::distance(centerA, centerB);
	float radii = radiusA + radiusB;

	if (distance >= radii)
		return false;

	normal = math::normalize(centerB - centerA);
	depth = radii - distance;

	return true;
}

void collision::removeSpaces(std::string& str) {
	std::string newStr;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] != ' ') {
			std::string temp(1, str[i]);
			newStr.append(temp);
		}
	}

	str = newStr;
}

std::string collision::getIdentifier(std::string str) {
	std::string identifier;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] != '(') {
			std::string temp(1, str[i]);
			identifier.append(temp);
		} else {
			return identifier;
		}
	}

	return identifier;
}

void collision::showCollisionBoxes(SDL_Renderer* renderer, std::vector<Fcollision*> allCollision) {
	bool showCurr = false;
	if (showCurr) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (int i = 0; i < temp.size(); i++) {
			SDL_Point* asdf = new SDL_Point[int(temp[i].size()) + 1];
			for (int j = 0; j < temp[i].size(); j++) {
				vector screenPoint = math::worldToScreen({ float(temp[i][j].x), float(temp[i][j].y) });
				asdf[j] = { int(screenPoint.x), int(screenPoint.y) };
			}

			vector start = math::worldToScreen({ float(temp[i][0].x), float(temp[i][0].y) });
			asdf[temp[i].size()] = { int(start.x), int(start.y) };

			SDL_RenderDrawLines(Main::renderer, asdf, int(temp[i].size()) + 1);
		}
	} else {
		// shows collision boxes
		for (int i = 0; i < allCollision.size(); i++) {
			if (allCollision[i]->isCircle)
				continue;

			SDL_Point* points = new SDL_Point[5];

			for (int j = 0; j < 4; j++)
				points[j] = { int(allCollision[i]->points[j].x), int(allCollision[i]->points[j].y) };

			points[4] = { int(allCollision[i]->points[0].x), int(allCollision[i]->points[0].y) };

			if (allCollision[i]->identifier == "")
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);


			math::pointsToScreen(points);
			SDL_RenderDrawLines(renderer, points, 5);
		}
	}
}

void collision::testCollisions(Fcollision* playerCol, std::vector<Fcollision*> allCollision, float deltaTime) {
	temp.clear();

	// mousePos
	vector mousePos = math::screenToWorld(Main::mousePos);
	std::unique_ptr<Fcollision> aCol = std::make_unique<Fcollision>(mousePos, 1, "");
	Main::mouseOverWater = false;

	for (int i = 0; i < allCollision.size(); i++) {
		// player col shouldn't exist any more, can replace it by converting the cirlce to a square
		vector normal = { 0, 0 };
		float depth = 0;
		if (Main::character->getCanMove()) {
			if (isCloseEnough(playerCol, allCollision[i])) {
				if ((allCollision[i]->isCircle && intersectCircles(playerCol->points[0], playerCol->radius, allCollision[i]->points[0], allCollision[i]->radius, normal, depth)) || intersectCirclePolygon(playerCol->points[0], playerCol->radius, allCollision[i]->points, normal, depth)) {
					// pushes back player from collision
					SaveData::saveData.playerLoc.x += -normal.x * depth;
					SaveData::saveData.playerLoc.y += -normal.y * depth;
				}
			}
		}

		if (!Main::mouseOverWater && allCollision[i]->identifier == "w" && isCloseEnough(aCol.get(), allCollision[i]) && intersectCirclePolygon(mousePos, 1, allCollision[i]->points, normal, depth))
			Main::mouseOverWater = true;
	}
}

// tests if player is colliding and pushes them back
void collision::testPlayerCollision(Fcollision* playerCol, std::vector<Fcollision*> allCollision) {
	temp.clear();
	if (!Main::character->getCanMove())
		return;

	for (int i = 0; i < allCollision.size(); i++) {
		// player col shouldn't exist any more, can replace it by converting the cirlce to a square
		if (isCloseEnough(playerCol, allCollision[i])) {
			vector normal;
			float depth;
			if ((allCollision[i]->isCircle && intersectCircles(playerCol->points[0], playerCol->radius, allCollision[i]->points[0], allCollision[i]->radius, normal, depth)) || intersectCirclePolygon(playerCol->points[0], playerCol->radius, allCollision[i]->points, normal, depth)) {
				// pushes back player from collision
				SaveData::saveData.playerLoc.x += -normal.x * depth;
				SaveData::saveData.playerLoc.y += -normal.y * depth;
			}
		}
	}
}

// test if the player is close enough for the collision to even be tested
// this doesn't reallistically needed to be calculated every frame, could do this once at the beginning and save it in another collision list, it would contain all the min and maxs, should make a collision struct
bool collision::isCloseEnough(const Fcollision* a, const Fcollision* col) {
	if (a->minX < col->maxX && a->maxX > col->minX && a->minY < col->maxY && a->maxY > col->minY) {
		temp.push_back(col->points);
		return true;
	}
	return false;
}