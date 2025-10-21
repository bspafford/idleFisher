#pragma once

#include <iostream>
#include <span>

class vector {
public:
	float x = 0;
	float y = 0;

	static vector zero() {
		return { 0, 0 };
	}

	float length() {
		return sqrt(x * x + y * y);
	}

	vector round() {
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

	bool operator== (vector other) {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!= (vector other) {
		return this->x != other.x || this->y != other.y;
	}

	friend std::ostream& operator<<(std::ostream& os, const vector& v) {
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}
};