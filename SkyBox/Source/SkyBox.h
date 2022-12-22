#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Shader.h"
#include "Mesh.h"

class SkyBox
{
public:
	SkyBox();

	SkyBox(std::vector<std::string> fileLocations);

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~SkyBox();
private:
	Mesh* skyMesh = nullptr;
	Shader* skyShader = nullptr;

	GLuint textureID = 0;
	GLuint uniformProjectionLocation = 0;
	GLuint uniformViewLocation = 0;
};