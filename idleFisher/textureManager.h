#pragma once

#include "math.h"

#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

struct textureStruct {
	unsigned char* texture = NULL;
	// if useAlpha == true then if mouse over a == 0 then it won't trigger
	bool useAlpha = false;
	int w = 0;
	int h = 0;
	int nChannels;

	textureStruct(unsigned char* bytes, bool _useAlpha, int _w, int _h, int _nChannels)
		: texture(bytes), useAlpha(_useAlpha), w(_w), h(_h), nChannels(_nChannels) {}
	~textureStruct() {
		delete texture;
	}
};

class textureManager {
public:
	textureManager();
	static void Deconstructor();
	static textureStruct* loadTexture(std::string path, bool loadSurface = false);
	static textureStruct* getTexture(std::string name);

private:
	static inline std::unordered_map<std::string, std::unique_ptr<textureStruct>> textureMap;
};