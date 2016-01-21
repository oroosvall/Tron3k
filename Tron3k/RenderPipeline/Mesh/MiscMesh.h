#ifndef MISCMESH_H
#define MISCMESH_H

#include <string>
#include <GL/glew.h>
#include "..\Map\MapHeaders.h"

struct StaticMeshHeader
{
	uint32_t materialCount;
	uint32_t textureCount;
	uint32_t indexCount;
	uint32_t vertexCount;
};

class MiscMesh
{
private:

	int* matOffsets;
	int* indices;
	Vertex11* verts;

	GLuint vbo;
	GLuint ibo;
	GLuint vao;

	TextureStruct* tex;
	int textureCount;

	Material* materials;

	int indexCount;
	
public:

	void init();
	void release();
	void load(std::string fileName);

	void draw();

};

#endif