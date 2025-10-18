#pragma once

#include "math.h"
#include "Hoverable.h"

class Shader;
class Image;

class dumpster : public IHoverable {
public:
	dumpster(vector loc);

	void onHover();
	void sellFish();
	void draw(Shader* shaderProgram);
	
private:
	bool bMouseOver = false;

	std::unique_ptr<Image> img;

	vector loc;
};