#include "Light.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/compatibility.hpp>

Light::Light()
{
	lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity,
			 GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntencity)
{
	lightColour = glm::vec3(red, green, blue);
	ambientIntensity = intensity;
	diffuseIntensity = dIntencity;
	direction = glm::vec3(xDir, yDir, zDir);
}

void Light::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
					 GLint dIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, lightColour.x, lightColour.y, lightColour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(dIntensityLocation, diffuseIntensity);
}

void Light::KeyControl(bool* keys, GLfloat deltaTime)
{
	if (!keys)
	{
		return;
	}

	if (keys[GLFW_KEY_1])
	{
		
		lightColour = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	if (keys[GLFW_KEY_2])
	{
		lightColour = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	if (keys[GLFW_KEY_3])
	{
		lightColour = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	if (keys[GLFW_KEY_4])
	{
		lightColour = glm::vec3(1.0f, 1.0f, 0.0f);
	}

	if (keys[GLFW_KEY_5])
	{
		lightColour = glm::vec3(1.0f, 0.0f, 1.0f);
	}

	if (keys[GLFW_KEY_6])
	{
		lightColour = glm::vec3(0.0f, 1.0f, 1.0f);
	}

	if (keys[GLFW_KEY_0])
	{
		lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	GLfloat addInts = 1.0f * deltaTime;

	if (keys[GLFW_KEY_MINUS])
	{
		AddIntensity(-addInts);
	}

	if (keys[GLFW_KEY_EQUAL])
	{
		AddIntensity(addInts);
	}
	//printf("v1: %.4f\n", ambientIntensity);
}

GLfloat Light::GetAmbientIntensity()
{
	//return ambientIntensity;
	return 0.0f;
}

Light::~Light()
{
}

void Light::AddIntensity(GLfloat add)
{
	ambientIntensity += add;
	if (ambientIntensity > 1.0f)
	{
		ambientIntensity = 1.0f;
	}
	else
	{
		if (ambientIntensity < 0.0f)
		{
			ambientIntensity = 0.0f;
		}
	}
}