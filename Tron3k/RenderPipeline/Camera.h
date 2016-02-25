#ifndef CAMERA_H
#define CAMERA_H


#include <glm\glm.hpp>
#include <GL\glew.h>

using namespace glm;

class Camera
{
private:

	mat4 viewMat;
	mat4 projMat;

	mat4 vp;

public:

	void init(int xres, int yres);

	void changeFoV(float newFoV);

	glm::mat4* getViewMat();

	void updateVP();
	void setProjMat(GLuint shader, GLuint shaderLocation);
	void setViewMat(GLuint shader, GLuint shaderLocation);
	void setViewProjMat(GLuint shader, GLuint shaderLocation);

	
};

#endif