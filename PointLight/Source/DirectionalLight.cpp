#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat intensity, GLfloat dIntencity,
	                               GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, intensity, dIntencity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
	GLint dIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(dIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}
