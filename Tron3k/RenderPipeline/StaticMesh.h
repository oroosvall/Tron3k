#ifndef STATICMESH_H
#define STATICMESH_H

#include <glm\glm.hpp>
#include <GL\glew.h>

#include "defines.h"

class StaticMesh
{
private:

	int materialCount;

	int* roomIDs;

	
	int vertexCount;
	Vertex11* verts;

	Index* indices;

public:

	int instanceCount;
	glm::mat4* worldMatrices;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArray;

	int indexCount;

	void release();

	void init(int instanceCount);
	void setRoomIDs(int* &room);
	void setMatrices(glm::mat4* &worldMatrices);
	void setVertices(float* &vertices, int vertexCount);
	void setIndices(int* &ndices, int indicesCount);

	void stream();

};

#endif