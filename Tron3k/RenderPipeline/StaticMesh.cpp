#include "StaticMesh.h"

void StaticMesh::init(int iCount)
{
	instanceCount = iCount;
}

void StaticMesh::setRoomIDs(int* &room)
{
	roomIDs = room;
}

void StaticMesh::setMatrices(glm::mat4* worldMats)
{
	worldMatrices = worldMats;
}

void StaticMesh::setVertices(float* vertices, int vertCount)
{
	vertexCount = vertCount;
	verts = (Vertex11*)vertices;
}

void StaticMesh::setIndices(int* inds, int indCount)
{
	indexCount = indCount;
	indices = (Index*)inds;
}