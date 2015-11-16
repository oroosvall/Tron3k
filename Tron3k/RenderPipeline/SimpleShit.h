#ifndef SIMPLESHIT_H
#define SIMPLESHIT_H

#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

class TestMesh
{
public:
	~TestMesh();
	GLuint vbuffer;
	GLuint vao;
	GLuint index;

	void make();
	void take(GLuint &vbuffer,
		GLuint &vao,
		GLuint &index,
		int &faceCount)
	{
		vbuffer = this->vbuffer;
		vao = this->vao;
		index = this->index;
		faceCount = this->faceCount;
	}

	//added
	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
	GLuint textureId;
	int faceCount;
	bool loadVert(string path);
	bool loadBMP(string path);
};

#endif