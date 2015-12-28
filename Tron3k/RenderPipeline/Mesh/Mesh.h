#ifndef MESH_H
#define MESH_H

#include "..\Utils\GPUMemoryLeakChecker.h"
#include <glm\glm.hpp>

#include "../defines.h"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>


class Mesh
{

private:

	Vertex* verts;
	Index* indices;

	unsigned int fileOffsetPtr;
	bool loaded;
	bool onGPU;
	
	glm::mat4 world;

public:

	~Mesh();
	void release();

	GLuint vbo;
	GLuint vao;
	GLuint ibo;

	GLuint textureID;

	int faceCount;

	void init(float x, float y, float z);
	void setTexture(GLuint textureID);
	void load(std::string file);

	float* getWorld();

	std::vector<float> getBoundaries();

};

#endif