#ifndef CAMERAINPUT_H
#define CAMERAINPUT_H

#include <glm\glm.hpp>
#include "Input.h"

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

using namespace glm;

class CameraInput
{

private:
	static CameraInput* singleton;
	CameraInput();

	Input* i;

	glm::mat4* viewMat;

	vec3 pos;
	vec3 dir;
	vec3 start;

	float playbackSpeed = 1.0f;

	int centerX, centerY;

	float angleH;
	float angleV;

	static float mouseSpeed; //higher val = faster
	float camSpeed = 10.0f; // higher val = faster

	mat3 rotH;
	mat3 rotV;

	double x_new, y_new;
	double x_last, y_last;

	void mousepan(float x, float y);
	void keypan(float dt);

public:
	static CameraInput* getCam();

	void setSensitivity(float sens) { mouseSpeed = sens; };
	float getSensitivity() { return mouseSpeed; };

	void init(glm::mat4* viewMat, int _xres, int _yres);
	//dont need to do this when we have a real crosshair in the game
	void setScreenCenter(int centerx, int centery);

	void update(float dt, bool freeCam);

	glm::vec3 getDir() { return dir; };
	glm::vec3 getPos() { return pos; };
	void setCam(vec3 _pos, vec3 _dir);
	void setCam(vec3 _pos);

	void setPlaybackSpeed(float val) { playbackSpeed = val; };

	glm::mat4* getViewMat() { return viewMat; };

	glm::mat4 getSkyboxMat();

	void getAngles(float &vertical, float &horizontal) { vertical = angleV; horizontal = angleH; };

	static void release();

	int roomID = 0;
};

#endif