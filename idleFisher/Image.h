#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include <stb/stb_image.h>
#include <vector>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include"shaderClass.h"

#include "math.h"

struct anchor {
	static inline std::string left = "left";
	static inline std::string right = "right";
	static inline std::string top = "top";
	static inline std::string bottom = "bottom";
	static inline std::string center = "center";
};

class Image {
public:
	GLuint ID;
	GLenum type;
	Image(std::string image, vector loc, bool useWorldPos);
	Image(Image* image, Rect* source, vector loc, bool useWorldPos);
	~Image();

	void draw(Shader* shaderProgram);
	void setSourceRect(Rect* rect);
	void setLoc(vector loc);
	vector getLoc();
	vector getAbsoluteLoc();
	void setRotation(float rot);

	bool isMouseOver();

	vector getSize();
	void setSize(vector scale);
	void setImage(std::string path);
	void setUseAlpha(bool useAlpha);
	void setColorMod(glm::vec4 colorMod);

	void setAnchor(std::string xAnchor, std::string yAnchor);
	void flipHoizontally(bool flip);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	GLenum texType = GL_TEXTURE_2D;
	GLenum pixelType = GL_UNSIGNED_BYTE;

	VAO* currVAO;
	//VBO* currVBO;
	// temp
	GLuint VBOId;
	EBO* currEBO;

	bool useWorldPos;
	bool useAlpha;

	float w = 0, h = 0;
	// scale of image without a source rect
	float ogW = 0, ogH = 0;
	Rect* source;
	float rotation;
	unsigned char* texture;
	GLenum textureFormat;
	std::string path;


private:
	std::vector<float> getPositionsList();

	void updatePositionsList(std::vector<float> positions = std::vector<float>(0));

	vector loc; // relative to the screen position
	vector absoluteLoc; // absolute position in the screen
	std::string xAnchor = anchor::left;
	std::string yAnchor = anchor::top;
	
	glm::vec4 colorMod = glm::vec4(1.f);

	bool flipHoriz = false;
};
#endif