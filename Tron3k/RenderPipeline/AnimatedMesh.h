#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include <string>
#include <glm\glm.hpp>
#include "Map\MapHeaders.h"
#include <GL/glew.h>

class AnimatedMesh
{

private:

	int* matOffsets;
	int* indices;
	AnimVertex* verts;
	int* animationKeyCounts;
	int* animationType;

	GLuint vbo;
	GLuint ibo;
	GLuint vao;

	int indexCount;

	int animationCount;
	int jointCount;

	Animation* animations;

public:

	void init();

	~AnimatedMesh();

	void load(std::string fileName);

	void draw();
};

#endif