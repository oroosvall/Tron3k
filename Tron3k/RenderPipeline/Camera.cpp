#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

void Camera::init()
{
	i = Input::getInput();
	i->getCursor(x_last, y_last);

	viewMat = glm::lookAt(glm::vec3(0, 0, 25), glm::vec3(0, 0, 24), glm::vec3(0, 1, 0));
	projMat = glm::perspective(3.14f*0.45f, 640.f / 480.0f, 0.1f, 1000.0f);

	pos = vec3(0, 0, 25);
	dir = vec3(0, 0, -1);
	
}

void Camera::update(float dt)
{
	i->getCursor(x_new, y_new);

	if (i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
		mousepan(float (x_new - x_last), float (y_new - y_last));

	keypan(dt);

	viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));

	x_last = x_new;
	y_last = y_new;
}

void Camera::keypan(float dt)
{
	float speedorig = camSpeed;
	if (i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
		camSpeed *= 9;
	camSpeed *= dt;

	if (i->getKeyInfo(GLFW_KEY_W))
		pos += dir * camSpeed;

	if (i->getKeyInfo(GLFW_KEY_S))
		pos -= dir * camSpeed;

	if (i->getKeyInfo(GLFW_KEY_A))
	{
		vec3 left = cross( vec3(0, 1, 0), dir);
		left = normalize(left);
		pos += left * camSpeed;
	}
	if (i->getKeyInfo(GLFW_KEY_D))
	{
		vec3 left = cross(dir, vec3(0, 1, 0));
		left = normalize(left);
		pos += left * camSpeed;
	}
	if (i->getKeyInfo(GLFW_KEY_SPACE))
		pos.y += camSpeed;

	if (i->getKeyInfo(GLFW_KEY_LEFT_CONTROL))
		pos.y -= camSpeed;

	camSpeed = speedorig;
}

void Camera::mousepan(float x, float y)
{
	angleH += x / mouseSpeed;
	angleV += y / mouseSpeed;
	if (angleV > 89)
		angleV = 89;
	if (angleV < -89)
		angleV = -89;

	//rotate vertically around x
	float rotateRad;
	vec3 view = vec3(1.0f, 0.0f, 0.0f);
	rotateRad = toRADIAN * angleV;

	rotV = mat3(cos(rotateRad), -sin(rotateRad), 0.0f,
				sin(rotateRad), cos(rotateRad), 0.0f,
				0.0f, 0.0f, 1.0f);
	view = rotV * view;
	view = normalize(view);

	//rotate horizontally around y
	rotateRad = toRADIAN * angleH;
	rotH = mat3(cos(rotateRad), 0.0f, -sin(rotateRad),
				0.0f, 1.0f, 0.0f,
				sin(rotateRad), 0.0f, cos(rotateRad));
	view = rotH * view;
	view = normalize(view);

	dir = pos - view;
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

