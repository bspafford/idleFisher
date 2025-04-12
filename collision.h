#pragma once

#include <string>

#include "math.h"

struct Fcollision {
	std::vector<vector> points;
	std::string identifier;
	bool isCircle;
	float radius;

	float minX;
	float maxX;
	float minY;
	float maxY;

	int pointNum = 4;

	// for polygon collision
	Fcollision(std::vector<vector> worldPoints, std::string identifier);

	// for circle collision
	Fcollision(vector center, float radius, std::string identifier);
	
};

class collision {
public:
	collision();

	// calculates all the collision.col file information into a list
	static void getCollisionObjects(std::vector<Fcollision*>& allCollision);
	static std::string getIdentifier(std::string str);

	// circles vs polygons
	static bool intersectCirclePolygon(vector circleCenter, float circleRadius, std::vector<vector> vertices, vector& normal, float& depth);
	static int findClosestPointOnPolygon(vector circleCenter, std::vector<vector> vertices);
	static void projectCircle(vector center, float radius, vector axis, float& min, float& max);

	// polygons vs polygons
	static bool intersectPolygons(std::vector<vector> verticesA, std::vector<vector> verticesB, vector& normal, float& depth);
	static void projectVertices(std::vector<vector> vertices, vector axis, float& min, float& max);

	// circles vs circles
	static bool intersectCircles(vector centerA, float radiusA, vector centerB, float radiusB, vector& normal, float& depth);

	static bool mouseOverWater(vector mousePos, std::vector<Fcollision*> allCollision);

	static void removeSpaces(std::string& str);

	static void showCollisionBoxes(SDL_Renderer* renderer, std::vector<Fcollision*> allCollision);

	// checks both mouse and player collision instead of having to loop through allCollisions multiple times
	static void testCollisions(Fcollision* playerCol, std::vector<Fcollision*> allCollision, float deltaTime);
	static void testPlayerCollision(Fcollision* playerCol, std::vector<Fcollision*> allCollision);
	static bool isCloseEnough(const Fcollision* a, const Fcollision* col/*std::vector<vector> b*/);

	static vector findArithmeticMean(std::vector<vector> vertices);


	// temp
	static inline std::vector<std::vector<vector>> temp;
};
