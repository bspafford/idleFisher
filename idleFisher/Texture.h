#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture {
public:
	GLuint ID;
	GLuint unit;

	Texture(const char* image);
	~Texture();

	// Assigns a texture unit to a texture
	void texUnit(Shader* shader, const char* uniform);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	static inline std::vector<std::unique_ptr<Texture>> textureCache;
	
	// binding stuff
	static void bindTextureToShader(Shader* shaderProgram, const char* path, const char* uniform);
	static void bindTextureToShader(std::vector<Shader*> shaderPrograms, const char* path, const char* uniform);
	static void deleteCache();

private:
	// false means slot isn't used, true means slot is currently being used
	static inline std::vector<bool> usedSlots;
	static GLuint takeOpenSlot();
	static void releaseSlot(GLuint slot);
};