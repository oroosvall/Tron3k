#ifndef SIMPLESHIT_H
#define SIMPLESHIT_H

#include <GL/glew.h>

class TestMesh
{
public:

	GLuint vbuffer;
	GLuint vao;
	GLuint index;

	void make();

};

#endif