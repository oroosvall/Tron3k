#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm\gtc\matrix_transform.hpp>

void Camera::init()
{
	viewMat = glm::lookAt(glm::vec3(0, 0, 25), glm::vec3(0, 0, 24), glm::vec3(0, 1, 0));
	projMat = glm::perspective(3.14f*0.35f, 640.f / 480.0f, 0.1f, 1000.0f);

}

glm::mat4* Camera::getViewMat()
{
	return &viewMat;
}

void Camera::changeFoV(float newFov)
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

void Camera::setViewProjMat(GLuint shader, GLuint shaderLocation)
{
	glm::mat4 VP = projMat * viewMat;
	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, &VP[0][0]);
}

