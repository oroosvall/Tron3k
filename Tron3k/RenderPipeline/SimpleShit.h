#ifndef SIMPLESHIT_H
#define SIMPLESHIT_H

#include <GL/glew.h>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

struct TriangleVertex
{
	float x, y, z;
	float u, v;
};

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

	float minX, minY, minZ, maxX, maxY, maxZ;
	void setMaxAndMinPos(std::vector<TriangleVertex> verts);

	bool loadVert(string path);
	bool loadBMP(string path);
};

#endif