#include "math.h"
#include "stuff.h"
#include "saveData.h"
#include "main.h"
#include "camera.h"
#include "textureManager.h"

#include <time.h>

// setup stuff to draw image
Fimg::Fimg(std::string path1, vector loc) {
	// make sure no duplicates
	/*
	if (std::find(tempImages.begin(), tempImages.end(), path1) == tempImages.end()) // not in list
		tempImages.push_back(path1);

	for (int i = 0; i < tempImages.size(); i++)
		//std::cout << "textureMap.insert(loadTexture(\"" + tempImages[i] + "\"));" << std::endl;
		std::cout << "loadTexture(\"" + tempImages[i] + "\");" << std::endl;

	std::cout << "\n\n\n" << std::endl;
	//*/

	//textureManager::loadTexture(path1);
	path = path1;
	textureStruct* texStruct = textureManager::getTexture(path1);

	if (texStruct) { // if already in map
		texture = texStruct->texture;
		surface = texStruct->surface;
		w = texStruct->w;
		h = texStruct->h;
	//* 
	} else { // need to add to map
		std::cout << "couldn't find: " << path1 << std::endl;
		textureStruct* texStruct1 = textureManager::loadTexture(path1);
		texture = texStruct1->texture;
		surface = texStruct1->surface;
		w = texStruct1->w;
		h = texStruct1->h;
	//*/
	}

	this->loc = loc;
	//setImage(path1, loc);
}

Fimg::Fimg(SDL_Texture* texture, SDL_Rect* source, vector loc) {
	this->texture = texture;
	this->source = source;
	if (source) {
		w = source->w;
		h = source->h;
	} else {
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	}
	this->loc = loc;
}

Fimg::Fimg(textureStruct* textureStruct, vector loc) {
	texture = textureStruct->texture;
	surface = textureStruct->surface;
	w = textureStruct->w;
	h = textureStruct->h;
	this->loc = loc;
}

Fimg::~Fimg() {
	/*
	delete source;
	SDL_FreeSurface(surface);
	if (texture)
		SDL_DestroyTexture(texture);
	texture = nullptr;
	surface = nullptr;
	source = nullptr;
	*/
}

void Fimg::setImage(std::string path1) {
	path = path1;
	textureStruct* texStruct = textureManager::getTexture(path1);
	if (texStruct) {
		texture = texStruct->texture;
		w = texStruct->w;
		h = texStruct->h;
	}

	SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b); // probelm: i think this directly effects the texture, if im sharing the texture then it will effect all the other textures too
}

void Fimg::setImage(std::string path1, vector loc) {
	setImage(path1);
	this->loc = loc;
}

void Fimg::setImage(SDL_Texture* texture) {
	this->texture = texture;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b);
}

void Fimg::setImage(SDL_Texture* texture, vector size) {
	this->texture = texture;
	w = size.x;
	h = size.y;
	SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b);
}

void Fimg::setSourceRect(SDL_Rect* rect) {
	source = rect;
}

void Fimg::loadSurface(std::string path) {
	textureStruct* texStruct = textureManager::getTexture(path);
	if (texStruct)
		if (texStruct->surface)
			surface = texStruct->surface;
		else {
			texStruct->surface = IMG_Load(path.c_str());
			surface = texStruct->surface;
		}
}

void Fimg::setImgColorMod(int r, int g, int b) {
	colorMod = SDL_Color(r, g, b);
	if (texture)
		SDL_SetTextureColorMod(texture, r, g, b);
}

void Fimg::setRotation(float rotation) {
	this->rotation = rotation;
}

float Fimg::getRotation() {
	return rotation;
}

// draw image
void Fimg::draw(SDL_Renderer* renderer, bool useCharLoc) {
	SDL_Rect rect;
	if (useCharLoc) {
		vector worldLoc = math::worldToScreen(loc);
		rect = { int(round(worldLoc.x)), int(round(worldLoc.y)), int(w * stuff::pixelSize), int(h * stuff::pixelSize) };
	} else
		rect = { int(loc.x), int(loc.y), int(w * stuff::pixelSize), int(h * stuff::pixelSize) };
	//SDL_RenderCopy(renderer, texture, source, &rect);
	SDL_RenderCopyEx(renderer, texture, source, &rect, rotation, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
}

bool Fimg::mouseOver() {
	vector mousePos = Main::mousePos;
	vector size = getSize();
	if (mousePos.x >= loc.x && mousePos.x <= loc.x + size.x && mousePos.y >= loc.y && mousePos.y <= loc.y + size.y)
		return true;
	return false;
}

vector Fimg::getSize() {
	return vector{ float(w), float(h) } * stuff::pixelSize;
}

// returns the dot product of two 2d vectors
float math::dot(vector a, vector b) {
	return a.x * b.x + a.y * b.y;
}

// normalizes the vector
vector math::normalize(vector a) {
	float invLen = Q_rsqrt(a.x * a.x + a.y * a.y);
	
	if (a.x == 0 && a.y == 0)
		return { 0, 0 };

	return { a.x * invLen, a.y * invLen };

}

float math::length(vector value) {
	return Q_sqrt(value.x * value.x + value.y * value.y);
}


// returns the distance between 2 points
float math::distance(float x1, float y1, float x2, float y2) {
	return Q_sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
	
}

float math::distance(vector a, vector b) {
	return Q_sqrt(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
}

float math::min(float a, float b) {
	if (a < b) return a;
	return b;
}

float math::max(float a, float b) {
	if (a > b) return a;
	return b;
}

float math::lerp(float min, float max, float val) {
	float dif = max - min;
	return val * dif + min;
}

float math::randRange(float min, float max) {
	return (float)rand() / RAND_MAX * (max - min) + min;
}

int math::randRangeInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

float math::clamp(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

float math::Q_sqrt(float number) {
	return Q_rsqrt(number) * number;
}

float math::Q_rsqrt(float number) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y)); // adds extra accuracy

	return y;
}

// converts screen coords to world coords
vector math::screenToWorld(float x, float y, bool round) {
	if (round)
		return { floor((Main::camera->pos.x + x - Main::screenWidth / 2) / stuff::pixelSize), floor((Main::camera->pos.y + y - Main::screenHeight / 2) / stuff::pixelSize) };
	else 
		return { (Main::camera->pos.x + x - Main::screenWidth / 2) / stuff::pixelSize, (Main::camera->pos.y + y - Main::screenHeight / 2) / stuff::pixelSize };
}
vector math::screenToWorld(vector pos, bool round) {
	if (round)
		return { floor((Main::camera->pos.x + pos.x - Main::screenWidth / 2) / stuff::pixelSize), floor((Main::camera->pos.y + pos.y - Main::screenHeight / 2) / stuff::pixelSize) };
	else
		return vector{ (Main::camera->pos.x + pos.x - Main::screenWidth / 2) / stuff::pixelSize, (Main::camera->pos.y + pos.y - Main::screenHeight / 2) / stuff::pixelSize };
}

// converts world coords to screen coords
vector math::worldToScreen(float x, float y) {
	float screenX = x * stuff::pixelSize - Main::camera->pos.x + Main::screenWidth / 2;
	float screenY = y * stuff::pixelSize - Main::camera->pos.y + Main::screenHeight / 2;

	return { screenX, screenY };
}
vector math::worldToScreen(vector pos) {
	float screenX = pos.x * stuff::pixelSize - Main::camera->pos.x + Main::screenWidth / 2;
	float screenY = pos.y * stuff::pixelSize - Main::camera->pos.y + Main::screenHeight / 2;

	return { screenX, screenY };
}

// takes an array of points an converts them from world to screen position
void math::pointsToScreen(SDL_Point*& points) {

	SDL_Point* newPoints = new SDL_Point[5];

	for (int i = 0; i < 5; i++) {
		vector point = worldToScreen(points[i].x, points[i].y);
		newPoints[i] = { (int)point.x, (int)point.y };
	}

	points = newPoints;
}

SDL_Color math::GetPixelColor(SDL_Surface* pSurface, const int X, const int Y) {
	const int x = X / stuff::pixelSize;
	const int y = Y / stuff::pixelSize;

	int bpp = pSurface->format->BytesPerPixel;
	Uint8* p = (Uint8*)pSurface->pixels + y * pSurface->pitch + x * bpp;
	SDL_Color Color = { 0, 0, 0, 0 };

	// checks if within image
	if (bpp == 4 && x <= pSurface->w && y <= pSurface->h) {
		Uint32 PixelColor = p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
		SDL_GetRGBA(PixelColor, pSurface->format, &Color.r, &Color.g, &Color.b, &Color.a);
		return Color;
	}

	return Color;
}

std::chrono::system_clock::time_point math::getTimeFromString(std::string timeString) {
	// from string, back to time
	std::tm tm = {};
	std::istringstream ss(timeString);
	ss >> std::get_time(&tm, "%Y-%m-%d %X");
	
	time_t t = std::mktime(&tm) - _timezone;
	return std::chrono::system_clock::from_time_t(t);
}

std::string math::getStringFromTime(std::chrono::system_clock::time_point timePoint) {
	// get local time and convert to string
	// auto const time = std::chrono::current_zone()
	//	->to_local(timePoint);

	std::string timestring = std::format("{:%Y-%m-%d %X}", timePoint);

	return timestring;
}

int math::getWorldIndexFromName(std::string worldName) {
	for (int i = 0; i < SaveData::data.worldData.size(); i++)
		if (SaveData::data.worldData[i].worldName == worldName)
			return i;
	return -1;
}