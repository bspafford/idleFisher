#include "textureManager.h"
#include "main.h"

#include <fstream>
#include <sstream>
#include <algorithm>  // For std::transform
#include <cctype>     // For tolower

textureManager::textureManager() {
	std::ifstream colFile("./data/png_files.txt");
	if (colFile.is_open()) {
		std::string line;
		while (colFile.good()) {
			std::getline(colFile, line);

			loadTexture(line);
		}
	}
}

textureStruct* textureManager::loadTexture(std::string path, bool loadSurface) {
	SDL_Texture* texture = IMG_LoadTexture(Main::renderer, (path).c_str());
	SDL_Surface* surface = NULL;
	if (loadSurface)
		surface = IMG_Load(path.c_str());
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	std::unique_ptr<textureStruct> texStruct = std::make_unique<textureStruct>(texture, surface, w, h);

	textureMap[path] = std::move(texStruct);

	// std::cout << textureMap.size() << std::endl;

	return textureMap[path].get();
}

textureStruct* textureManager::getTexture(std::string name) {
	// to lower
	std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
		return std::tolower(c);
		});

	if (textureMap.find(name) != textureMap.end())
		return textureMap[name].get();
	else
		return NULL;
}