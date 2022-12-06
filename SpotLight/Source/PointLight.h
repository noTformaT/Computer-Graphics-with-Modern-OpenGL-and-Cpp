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

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint dIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentlLocation);

	void SetPosition(glm::vec3 newLocation);
	glm::vec3 GetLocation();

	~PointLight();

protected:
	glm::vec3 position;
	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;
};