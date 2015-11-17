#include "CameraInput.h"
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

CameraInput* CameraInput::singleton = nullptr;

CameraInput::CameraInput()
{

}

void CameraInput::init(glm::mat4* view)
{
	viewMat = view;

	i = Input::getInput();
	i->getCursor(x_last, y_last);

	start = vec3(-1, 0, 0);
	dir = start;

	setCam(vec3(0, 0, 25), vec3(0, -0.5, -1));
}

void CameraInput::update(float dt, bool freeCam)
{
	i->getCursor(x_new, y_new);
	
	if (i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
	{
		int x = x_new - x_last;
		int y = y_new - y_last;
		mousepan(x, y);
	}
	
	if(freeCam)
		keypan(dt);
	
	*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
	
	x_last = x_new;
	y_last = y_new;
}

void CameraInput::keypan(float dt)
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
		vec3 left = cross(vec3(0, 1, 0), dir);
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

void CameraInput::mousepan(float x, float y)
{
	angleH += x / mouseSpeed;
	angleV += y / mouseSpeed;
	if (angleV > 89)
		angleV = 89;
	if (angleV < -89)
		angleV = -89;

	//rotate vertically around x
	float rotateRad;
	vec3 view = start;
	rotateRad = toRADIAN * angleV;

	rotV = mat3(	cos(rotateRad),		-sin(rotateRad),	0.0f,
					sin(rotateRad),		cos(rotateRad),		0.0f,
					0.0f,				0.0f,				1.0f);
	view = rotV * view;
	view = normalize(view);

	//rotate horizontally around y
	rotateRad = toRADIAN * angleH;
	rotH = mat3(cos(rotateRad), 0.0f, -sin(rotateRad),
		0.0f, 1.0f, 0.0f,
		sin(rotateRad), 0.0f, cos(rotateRad));
	view = rotH * view;
	dir = normalize(view);

}

void CameraInput::setCam(vec3 _pos, vec3 _dir)
{
	_dir = normalize(_dir);

	float angleXZ = atan2(_dir.x, _dir.z) - atan2(dir.x, dir.z);
	angleH += angleXZ * toDEGREE;


	float angleYold = dot( dir, vec3(0, 1, 0) );
	float angleYnew = dot( _dir, vec3(0, 1, 0) );

	float diff = angleYold - angleYnew;
	diff *= toDEGREE;

	angleV -= diff;

	pos = _pos;
	dir = _dir;
}

CameraInput* CameraInput::getCam()
{
	if (singleton == nullptr)
		singleton = new CameraInput();
	return singleton;
}

void CameraInput::release()
{
	if (singleton != nullptr)
		delete singleton;
}