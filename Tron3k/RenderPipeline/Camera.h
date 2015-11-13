#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <GL\glew.h>

class Camera
{
private:

	glm::mat4 viewMat;
	glm::mat4 projMat;

public:

	void init();
	void update(glm::mat4* viewMat);

	void changeFoV(float newFoV);

	void setProjMat(GLuint shaderLocation);
	void setViewMat(GLuint shaderLocation);

};

#endif