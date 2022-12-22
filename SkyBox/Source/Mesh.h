#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVerticles, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();
	void Update();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};