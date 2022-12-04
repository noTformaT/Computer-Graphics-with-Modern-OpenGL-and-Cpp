#pragma once

#include <stdio.h>
#include <string>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation , const char* fragmentLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseDirection();
	GLuint GetDiffuseIntensity();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformAmbientIntensity;
	GLuint uniformAmbientColor;
	GLuint uniformDiffuseDirection;
	GLuint uniformDiffuseIntensity;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformEyePosition;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	std::string ReadFile(const char* fileLocation);
};