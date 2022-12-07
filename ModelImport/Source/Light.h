#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity,
		  GLfloat dIntencity);

	void KeyControl(bool* keys, GLfloat deltaTime);

	GLfloat GetAmbientIntensity();

	~Light();

protected:
	glm::vec3 lightColour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	void AddIntensity(GLfloat add);
};