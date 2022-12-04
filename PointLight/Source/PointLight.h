#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat intensity, GLfloat dIntencity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
		GLint dIntensityLocation, GLint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentlLocation);

	~PointLight();

private:
	glm::vec3 position;
	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;
};