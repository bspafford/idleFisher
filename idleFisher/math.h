#pragma once

#define M_PI 3.1415926535

#include <iostream>
#include <chrono>
#include <glm/glm.hpp>
#include <alpaca/alpaca.h>

class Image;

struct vector {
	float x = 0;
	float y = 0;

	static vector zero() {
		return { 0, 0 };
	}

	float length() {
		return sqrt(x * x + y * y);
	}

	vector round() {
		//return vector{ roundf(x), roundf(y) };
		return vector{ floorf(x), floorf(y) };
	}

	// Addition
	vector operator+ (const vector& other) const {
		return { x + other.x, y + other.y };
	}

	vector operator+ (float b) const {
		return { x + b, y + b };
	}

	vector operator+= (const vector& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	vector operator+= (float other) {
		x += other;
		y += other;
		return *this;
	}

	// Subtraction
	vector operator-(const vector& other) const {
		return { x - other.x, y - other.y };
	}

	vector operator- (float b) const {
		return { x - b, y - b };
	}

	vector operator-= (const vector& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vector operator-= (float other) {
		x -= other;
		y -= other;
		return *this;
	}

	// Unary minus
	vector operator-() const {
		return { -x, -y };
	}

	// Multiplication
	vector operator* (vector b) const {
		return { x * b.x, y * b.y };
	}

	vector operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}

	vector operator*= (vector other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	vector operator*= (float other) {
		x *= other;
		y *= other;
		return *this;
	}

	// Division
	vector operator/ (vector b) const {
		return { x / b.x, y / b.y };
	}

	vector operator/ (float scalar) const {
		return { x / scalar, y / scalar };
	}

	vector operator/= (vector other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	vector operator/= (float other) {
		x /= other;
		y /= other;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const vector& v) {
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}
};

struct Rect {
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	Rect() {}
	template<typename T>
	Rect(T _x, T _y, T _w, T _h) : x(static_cast<float>(_x)), y(static_cast<float>(_y)), w(static_cast<float>(_w)), h(static_cast<float>(_h)) {}

	friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
		os << "(" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << ")";
		return os;
	}
};

class math {
public:
	static float dot(vector a, vector b);
	static vector normalize(vector a);
	static float length(vector value);
	static float distance(float x1, float y1, float x2, float y2);
	static float distance(vector a, vector b);
	static float min(float a, float b);
	static float max(float a, float b);
	static float lerp(float min, float max, float val);
	static float randRange(float min, float max);
	static int randRangeInt(int min, int max);
	static float clamp(float x, float min, float max);
	static vector perp(vector v);

	// quick so its approximation
	static float Q_sqrt(float number);
	// does the inverse (1/sqrt)
	static float Q_rsqrt(float number);

	// helper functions
	static vector screenToWorld(vector pos, bool round = true);

	// in glfw, (0, 0) is the center of the screen
	// so stuff like images will already take account of this and have the origin in the center.
	// but stuff like normal coordinates will have to calculate this
	// can test this by setting objects coords to (0, 0) and see if they are either in the top left or in the center of the screen
	static vector worldToScreen(vector pos, std::string origin);

	// time stuff
	// Y-M-D h:m:s
	static std::chrono::system_clock::time_point getTimeFromString(std::string timeString);
	static std::string getStringFromTime(std::chrono::system_clock::time_point timePoint);

	static int getWorldIndexFromName(std::string worldName);

	// lowercases string
	static std::string toLower(std::string str);

	static void computeOrthoMatrix(int width, int height, float* matrix);
	static glm::vec2 convertToRelativeCoords(glm::vec3 movement);
	static glm::vec3 convertFromRelativeCoords(glm::vec2 relative);
};