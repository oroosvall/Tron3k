#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

void Camera::init()
{
	viewMat = glm::lookAt(glm::vec3(0, 0, 25), glm::vec3(0, 0, 24), glm::vec3(0, 1, 0));
	projMat = glm::perspective(3.14f * 0.45f, 1.0f, 0.1f, 1000.0f);
}

void Camera::update(glm::mat4* mat)
{

}

void Camera::setProjMat(GLuint shader, GLuint shaderLocation)
{
	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, &projMat[0][0]);
}

void Camera::setViewMat(GLuint shader, GLuint shaderLocation)
{
	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, &viewMat[0][0]);
}

void Camera::changeFoV(float newFov)
{

}