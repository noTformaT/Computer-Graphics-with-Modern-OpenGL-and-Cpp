#pragma once

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	virtual	void Write();

	virtual void Read(GLenum textureUnit);

	GLuint GetShadowWidth();
	GLuint GetShadowHeight();

private:
	GLuint FBO;
	GLuint shadowMap;
	GLuint shadowWidth;
	GLuint shadowHeight;
};

