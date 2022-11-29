#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLuint windoWidth, GLuint windowHeight);
	
	int Initialise();

	GLfloat GetBufferWidth();
	GLfloat GetBufferHeight();

	bool GetShouldClose();
	
	void SwapBuffers();
	
	~Window();

private:
	GLFWwindow* mainWindow;

	GLuint width;
	GLuint height;

	GLint bufferWidth;
	GLint bufferHeight;
};