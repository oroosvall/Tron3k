#ifndef MESH_H
#define MESH_H

#include <GL\glew.h>
#include <glm\glm.hpp>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

struct Vertex
{
	float x, y, z;
	float n1, n2, n3;
	float u, v;
};

struct Index
{
	unsigned int i1, i2, i3;
};

class Mesh
{

private:

	Vertex* verts;
	Index* indices;

	unsigned int fileOffsetPtr;
	bool loaded;
	bool onGPU;

	GLuint vbo;
	GLuint vao;
	GLuint ibo;
	
	glm::mat4 world;

public:

	void init();

	void load(std::string file);

	//void 

};

#endif