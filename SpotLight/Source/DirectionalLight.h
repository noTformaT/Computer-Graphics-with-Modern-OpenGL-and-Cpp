#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat intensity, GLfloat dIntencity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
		GLint dIntensityLocation, GLint directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};