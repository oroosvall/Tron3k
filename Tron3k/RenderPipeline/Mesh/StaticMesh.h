#ifndef STATICMESH_H
#define STATICMESH_H

#include <glm\glm.hpp>
#include "..\Utils\GPUMemoryLeakChecker.h"

#include "../defines.h"

class StaticMesh
{
private:

	int materialCount;
	
	int vertexCount;
	Vertex11* verts;

	Index* indices;

public:

	int material;

	int instanceCount;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArray;

	int indexCount;

	void release();

	void init(int instanceCount);
	void setVertices(float* &vertices, int vertexCount);
	void setIndices(int* &ndices, int indicesCount);

	void stream();

};

#endif