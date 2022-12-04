#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat intensity, GLfloat dIntencity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, intensity, dIntencity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint dIntensityLocation, GLint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentlLocation)
{
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(dIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentlLocation, exponent);
}

PointLight::~PointLight()
{
}
