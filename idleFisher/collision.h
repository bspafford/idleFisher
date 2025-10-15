#pragma once

#include <string>

#include "math.h"

class Shader;

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
	static void getCollisionObjects();
	static void removeCollisionObject(Fcollision* collision);
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

	static void showCollisionBoxes(Shader* shaderProgram);

	// checks both mouse and player collision instead of having to loop through allCollisions multiple times
	static void testCollisions(Fcollision* playerCol, std::vector<Fcollision*> allCollision);
	static void testPlayerCollision(Fcollision* playerCol, std::vector<Fcollision*> allCollision);
	static bool isCloseEnough(const Fcollision* a, const Fcollision* col);

	static vector findArithmeticMean(std::vector<vector> vertices);

	// CCD
	static bool sweepPointVsEdge(vector p0, vector v, vector edgeStart, vector edgeEnd, float radius, float* toiOut, vector* normalOut);
	static bool sweepPointVsCircle(vector p0, vector v, vector center, float radius, float* toiOut, vector* normalOut);
	static bool testCCD(Fcollision* playerCol, vector move, float deltaTime);
	static bool circleVsCircle(Fcollision* playerCol, vector v, Fcollision* circleCol, float* toiOut, vector* normalOut);

	static bool testMouse(vector mousePos);
	static bool pointInQuad(vector mousePos, Fcollision* col);
	static bool pointInTriangle(const vector& pt, const vector& a, const vector& b, const vector& c);

	static inline std::vector<Fcollision*> allCollision;
	// holds unique_ptr for allCollision
	static inline std::vector<std::unique_ptr<Fcollision>> collisionStorage;

	static inline std::vector<std::vector<vector>> temp;
};
