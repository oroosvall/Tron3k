#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm\gtc\matrix_transform.hpp>

void Camera::init(int xres, int yres)
{
	viewMat = glm::lookAt(glm::vec3(0, 0, 25), glm::vec3(0, 0, 24), glm::vec3(0, 1, 0));
	//projMat = glm::perspective(0.0174533f * 45.0f, float(xres) / float(yres), 0.1f, 1000.0f);
	float fov = 90.0f;
	float fovFactor = 60.0f / 180.0f;
	projMat = glm::perspectiveFov(3.14f * fovFactor, float(xres), float(yres), 0.05f, 1000.0f);
}

glm::mat4* Camera::getViewMat()
{
	return &viewMat;
}

void Camera::changeFoV(float newFov)
{

}

void Camera::updateVP()
{
	vp = projMat * viewMat;
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
	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, &vp[0][0]);
}

