#pragma once

#include "math.h"
#include "shaderClass.h"
#include "Image.h"

class URectangle {
public:
	URectangle(vector loc, vector size, bool useWorldLoc, glm::vec4 color = glm::vec4(1.f));
	~URectangle();

	void loadGPU();

	void draw(Shader* shaderProgram);
	void setColor(glm::vec4 color);
	void setLoc(vector loc);
	vector getLoc();
	void setSize(vector size);
	vector getSize();
	void setAnchor(std::string xAnchor, std::string yAnchor);
	void updatePositionsList();

private:
	vector size;
	glm::vec4 color = glm::vec4(1.f);
	bool useWorldLoc;

	vector loc; // relative to the screen position
	vector absoluteLoc; // absolute position in the screen
	std::string xAnchor = anchor::left;
	std::string yAnchor = anchor::top;

	GLuint VBOId;
	VAO* currVAO = nullptr;
	EBO* currEBO = nullptr;
};