#include "path.h"

Path::Path(std::vector<vector> waypoints, vector startPos, float turnDst) {
	lookPoints = waypoints;
	turnBoundaries = std::vector<Line*>(lookPoints.size());
	finishLineIndex = turnBoundaries.size() - 1;

	vector previousPoint = startPos;
	for (int i = 0; i < lookPoints.size(); i++) {
		vector currentPoint = lookPoints[i];
		vector dirToCurrentPoint = math::normalize(currentPoint - previousPoint);
		vector turnBoundaryPoint = (i == finishLineIndex) ? currentPoint : currentPoint - dirToCurrentPoint * turnDst;
		turnBoundaries[i] = new Line(turnBoundaryPoint, previousPoint - dirToCurrentPoint * turnDst);
		previousPoint = turnBoundaryPoint;
	}
}

void Path::draw(SDL_Renderer* renderer) {
	for (vector p : lookPoints) {
		vector loc = math::worldToScreen(p);
		SDL_Rect* rect = new SDL_Rect{ int(loc.x), int(loc.y), 4, 4 };
		SDL_RenderFillRect(renderer, rect);
	}
}