#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLcation);
	~Texture();

	bool LoadTexture(GLint format = GL_RGBA);
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;

	const char* fileLocation;
};