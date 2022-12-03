#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 initPosition, glm::vec3 initUp, GLfloat initYaw, GLfloat initPitch, GLfloat initMoveSpeed, GLfloat initTurnSpeed);
	
	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime);
	glm::mat4 CalculateVewMatrix();
	
	~Camera();

private:
	glm::vec3 position = glm::vec3(.0f);
	glm::vec3 front = glm::vec3(.0f);
	glm::vec3 up = glm::vec3(.0f);
	glm::vec3 right = glm::vec3(.0f);
	glm::vec3 worldUp = glm::vec3(.0f);

	GLfloat yaw = .0f;
	GLfloat pitch = .0f;
	GLfloat roll = .0f;

	GLfloat movementSpeed = .0f;
	GLfloat turnSpeed = .0f;

	void UpdateCamera();
};