#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	void KeyControl(bool* keys, GLfloat deltaTime);

	~Light();

private:
	glm::vec3 lightColour;
	GLfloat ambientIntensity;

	void AddIntensity(GLfloat add);
};