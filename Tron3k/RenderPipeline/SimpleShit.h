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

	//added
	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
	GLuint textureId;
	int faceCount;
	bool loadVert(string path);
	bool loadBMP(string path);
};

#endif