#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLcation);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;

	char* fileLocation;
};