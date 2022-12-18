#include "Light.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/compatibility.hpp>

Light::Light()
{
	lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity,
			 GLfloat dIntencity)
{
	lightColour = glm::vec3(red, green, blue);
	ambientIntensity = intensity;
	diffuseIntensity = dIntencity;

	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);

	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
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

ShadowMap* Light::GetShadowMap()
{
	return shadowMap;
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