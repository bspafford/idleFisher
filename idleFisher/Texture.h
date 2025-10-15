#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture {
public:
	GLuint ID;
	GLuint unit;

	Texture(const char* image, GLuint slot);
	~Texture();

	// Assigns a texture unit to a texture
	void texUnit(Shader* shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	static inline std::vector<std::unique_ptr<Texture>> textureCache;
	
	// binding stuff
	static inline GLuint bindIndex = 2;
	static void bindTextureToShader(Shader* shaderProgram, const char* path, const char* uniform);
	static void bindTextureToShader(std::vector<Shader*> shaderPrograms, const char* path, const char* uniform);
	static void deleteCache();
};