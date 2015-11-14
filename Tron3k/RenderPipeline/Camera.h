#ifndef CAMERA_H
#define CAMERA_H


#include <glm\glm.hpp>
#include <GL\glew.h>

#include "../Project/Core/Input.h"

#define toDEGREE 57.2957795f
#define toRADIAN 0.0174532925f

using namespace glm;

class Camera
{
private:
	Input* i;

	mat4 viewMat;
	mat4 projMat;

	vec3 pos;
	vec3 dir;

	float angleH;
	float angleV;

	float mouseSpeed = 5.0f; //higher val = slower
	float camSpeed = 10.0f; // higher val = faster

	mat3 rotH;
	mat3 rotV;
	
	double x_new, y_new;
	double x_last, y_last;

public:

	void init();
	void update(float dt);

	void mousepan(float x, float y);
	void keypan(float dt);

	void changeFoV(float newFoV);

	void setProjMat(GLuint shader, GLuint shaderLocation);
	void setViewMat(GLuint shader, GLuint shaderLocation);
	void setViewProjMat(GLuint shader, GLuint shaderLocation);

	
};

#endif