#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity,
		  GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntencity);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
				  GLint dIntensityLocation, GLint directionLocation);

	void KeyControl(bool* keys, GLfloat deltaTime);

	GLfloat GetAmbientIntensity();

	~Light();

private:
	glm::vec3 lightColour;
	GLfloat ambientIntensity;
	glm::vec3 direction;
	GLfloat diffuseIntensity;

	void AddIntensity(GLfloat add);
};