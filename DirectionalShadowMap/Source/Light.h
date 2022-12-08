#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "ShadowMap.h"
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight,
		  GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity,
		  GLfloat dIntencity);

	void KeyControl(bool* keys, GLfloat deltaTime);

	GLfloat GetAmbientIntensity();

	ShadowMap* GetShadowMap();

	~Light();

protected:
	glm::vec3 lightColour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	ShadowMap* shadowMap = nullptr;

	glm::mat4 lightProj;

	void AddIntensity(GLfloat add);
};