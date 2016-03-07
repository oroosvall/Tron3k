#include "CameraInput.h"
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

CameraInput* CameraInput::singleton = nullptr;

float CameraInput::mouseSpeed = 0.2f;

CameraInput::CameraInput()
{

}

void CameraInput::init(glm::mat4* view, int centerX, int centerY)
{
	setScreenCenter(centerX, centerY);

	viewMat = view;

	i = Input::getInput();
	i->getCursor(x_last, y_last);

	start = vec3(-1, 0, 0);
	dir = start;

	setCam(vec3(0, 0, 25), vec3(0, -0.5, -1));
}

void CameraInput::setScreenCenter(int _centerX, int _centerY)
{
	centerX = _centerX;
	centerY = _centerY;
}

void CameraInput::update(float dt, bool freeCam)
{
	i->getCursor(x_new, y_new);

	int x = x_new - centerX;
	int y = y_new - centerY;
	mousepan(x, y);
	
	if(freeCam)
		keypan(dt);
	
	if(freeCam)
		*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
	
	i->centerCursor(centerX, centerY);
}

void CameraInput::keypan(float dt)
{
	float speedorig = camSpeed;
	camSpeed /= playbackSpeed;

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
	angleH -= x * (mouseSpeed/5.0f * playbackSpeed);
	angleV -= y * (mouseSpeed/5.0f * playbackSpeed);
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

	*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
}

void CameraInput::setCam(vec3 _pos)
{
	pos = _pos;
	*viewMat = lookAt(pos, pos + dir, vec3(0, 1, 0));
}

CameraInput* CameraInput::getCam()
{
	if (singleton == nullptr)
		singleton = new CameraInput();
	return singleton;
}

glm::mat4 CameraInput::getSkyboxMat()
{
	

	glm::mat4 ret;
	//scale of skybox
	ret[0].x = 550;
	ret[1].y = 550;
	ret[2].z = 550;

	float rota = -96.5f * 0.0174533f;

	glm::mat4 rot = mat4(cos(rota), 0.0f, -sin(rota), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(rota), 0.0f, cos(rota), 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	ret *= rot;

	ret[0].w = pos.x;
	ret[1].w = pos.y;
	ret[2].w = pos.z;

	
	return ret;
}

void CameraInput::release()
{
	if (singleton != nullptr)
		delete singleton;
}