#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

void Camera::init()
{
	viewMat = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projMat = glm::perspective(75.0f, 1.0f, 0.1f, 100.0f);
}

void Camera::update(glm::mat4* mat)
{

}

void Camera::setProjMat(GLuint shaderLocation)
{

}

void Camera::setViewMat(GLuint shaderLocation)
{

}

void Camera::changeFoV(float newFov)
{

}