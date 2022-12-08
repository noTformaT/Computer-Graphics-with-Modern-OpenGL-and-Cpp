#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntencity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat dirX, GLfloat dirY, GLfloat dirZ,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint dIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentlLocation,
		GLuint edgeLocation);

	void SetPositionAndDirection(glm::vec3 newPos, glm::vec3 newDir);

	void AddDegress(GLfloat degress, glm::vec3 rotationVector);

	~SpotLight();

private:
	glm::vec3 direction;
	GLfloat edge;
	GLfloat procEdge;
};