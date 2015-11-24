#ifndef STATICMESH_H
#define STATICMESH_H

#include <glm\glm.hpp>
#include <GL\glew.h>

#include "defines.h"

class StaticMesh
{
private:

	int instanceCount;
	int materialCount;

	int* roomIDs;
	glm::mat4* worldMatrices;
	
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArray;
	
	int vertexCount;
	Vertex11* verts;

	Index* indices;
	int indexCount;

public:

	void init(int instanceCount);
	void setRoomIDs(int* &room);
	void setMatrices(glm::mat4* worldMatrices);
	void setVertices(float* vertices, int vertexCount);
	void setIndices(int* indices, int indicesCount);
};

#endif