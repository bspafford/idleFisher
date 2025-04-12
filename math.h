#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <chrono>

#include "stuff.h"

struct textureStruct;

struct vector {
	float x = 0;
	float y = 0;
};

// image stuff
struct Fimg {
	std::string path;
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;
	int w, h;
	vector loc;

private:
	SDL_Rect* source = NULL;

	SDL_Color colorMod = { 255, 255, 255 };
	float rotation = 0;

public:
	Fimg(std::string path, vector loc = { 0, 0 });
	Fimg(SDL_Texture* texture, SDL_Rect* source, vector loc);
	Fimg(textureStruct* textureStruct, vector loc = { 0, 0 });
	~Fimg();
	void loadSurface(std::string path);
	void setImage(std::string path);
	void setImage(std::string path, vector loc);
	void setImage(SDL_Texture* texture);
	void setImage(SDL_Texture* texture, vector size);
	void setSourceRect(SDL_Rect* rect);
	void setImgColorMod(int r, int g, int b);
	void draw(SDL_Renderer* renderer, bool useCharLoc = true);
	bool mouseOver();
	vector getSize();
	void setRotation(float rotation);
	float getRotation();

	static inline std::vector<std::string> tempImages;
};

// operator overloading
inline vector operator+ (vector a, vector b) {
	return { a.x + b.x, a.y + b.y };
}

inline vector operator+ (vector a, float b) {
	return { a.x + b, a.y + b };
}

inline vector operator- (vector a, vector b) {
	return { a.x - b.x, a.y - b.y };
}

inline vector operator- (vector a, float b) {
	return { a.x - b, a.y - b };
}

inline vector operator* (vector a, float b) {
	return { a.x * b, a.y * b };
}

inline vector operator* (vector a, vector b) {
	return { a.x * b.x, a.y * b.y };
}

inline vector operator*= (vector a, float b) {
	return { a.x * b, a.y * b };
}

inline vector operator/ (vector a, float b) {
	return { a.x / b, a.y / b };
}

inline std::ostream& operator<< (std::ostream& os, vector& a) {
	os << a.x << ", " << a.y;
	return os;
}

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

	// quick so its approximation
	static float Q_sqrt(float number);
	// does the inverse (1/sqrt)
	static float Q_rsqrt(float number);

	// helper functions
	static vector screenToWorld(float x, float y, bool round = true);
	static vector screenToWorld(vector pos, bool round = true);

	static vector worldToScreen(float x, float y);
	static vector worldToScreen(vector pos);

	static void pointsToScreen(SDL_Point*& points);

	// sdl stuff
	static SDL_Color GetPixelColor(SDL_Surface* pSurface, const int X, const int Y);

	// time stuff
	// Y-M-D h:m:s
	static std::chrono::system_clock::time_point getTimeFromString(std::string timeString);
	static std::string getStringFromTime(std::chrono::system_clock::time_point timePoint);

	static int getWorldIndexFromName(std::string worldName);
};