#include "building.h"
#include "collision.h"
#include "Acharacter.h"
#include "main.h"

Abuilding::Abuilding(vector loc) {
	this->loc = loc;
}

Abuilding::~Abuilding() {

}

void Abuilding::draw(SDL_Renderer* renderer) {
	buildingImg->draw(renderer);
}
 
void Abuilding::setLoc(vector loc) {
	this->loc = loc;
	buildingImg->loc = loc;
}

void Abuilding::setupCollision(std::vector<vector> pointsList) {
	col = new Fcollision(pointsList, " ");
	Main::allCollision.push_back(col);

	float minIndex = -1, minVal = INFINITY;
	float maxIndex = -1, maxVal = -INFINITY;
	for (int i = 0; i < col->points.size(); i++) {
		if (col->points[i].x < minVal) {
			minVal = col->points[i].x;
			minIndex = i;
		} else if (col->points[i].x > maxVal) {
			maxVal = col->points[i].x;
			maxIndex = i;
		}
	}

	leftPoint = col->points[minIndex];
	rightPoint = col->points[maxIndex];
}

bool Abuilding::calcInFront() {
	return false;
	vector charLoc = Acharacter::getCharLoc();
	vector p1 = leftPoint;
	vector p2 = rightPoint;
	float m = (p2.y - p1.y) / (p2.x - p1.x);
	float y = m * (charLoc.x - p1.x) + p1.y;

	return charLoc.y <= y;
}