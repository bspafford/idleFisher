#pragma once

#include "math.h"

#include <unordered_map>
#include <memory>

struct textureStruct {
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;
	int w = 0;
	int h = 0;

	textureStruct(SDL_Texture* _texture, SDL_Surface* _surface, int _w, int _h)
		: texture(_texture), surface(_surface), w(_w), h(_h) {
	}
};

class textureManager {
public:
	textureManager();
	static textureStruct* loadTexture(std::string path, bool loadSurface = false);
	static textureStruct* getTexture(std::string name);

private:
	static inline std::unordered_map<std::string, std::unique_ptr<textureStruct>> textureMap;
};