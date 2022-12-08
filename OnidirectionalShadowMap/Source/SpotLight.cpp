#include "SpotLight.h"
#include "glm/gtx/rotate_vector.hpp"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat aIntensity, GLfloat dIntencity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat dirX, GLfloat dirY, GLfloat dirZ, 
	GLfloat con, GLfloat lin, GLfloat exp, 
	GLfloat edg) : PointLight(red, green, blue, aIntensity, dIntencity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::normalize(glm::vec3(dirX, dirY, dirZ));
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint dIntensityLocation, 
	GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, 
	GLuint exponentlLocation, GLuint edgeLocation)
{
	// base light
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(dIntensityLocation, diffuseIntensity);

	// point light
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentlLocation, exponent);

	// spot light
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetPositionAndDirection(glm::vec3 newPos, glm::vec3 newDir)
{
	position = newPos;
	direction = newDir;
}

SpotLight::~SpotLight()
{
}

void SpotLight::AddDegress(GLfloat degress, glm::vec3 rotationVector)
{
	direction = glm::rotate(direction, degress, rotationVector);
	/*rotationVector = glm::rotateX(rotationVector, degress);
	direction += rotationVector;
	direction = normalize(direction);*/
}
