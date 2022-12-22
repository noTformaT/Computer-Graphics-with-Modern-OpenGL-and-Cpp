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
	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange();

	bool* GetKeys();

	void UpdateMouse();

	~Window();

private:
	GLFWwindow* mainWindow;

	GLuint width;
	GLuint height;

	GLint bufferWidth;
	GLint bufferHeight;

	// KeyBoard
	bool keys[1024];
	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	// Mouse
	GLfloat lastX = .0f;
	GLfloat lastY = .0f;
	GLfloat xChange = .0f;
	GLfloat yChange = .0f;
	bool mouseFirstMoved = true;
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};