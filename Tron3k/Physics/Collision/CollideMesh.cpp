#include"CollideMesh.h"

CollideMesh::CollideMesh()
{

}

CollideMesh::~CollideMesh()
{

}

void CollideMesh::addAABB(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize)
{
	boundingBox.posX = xPos;
	boundingBox.posY = yPos;
	boundingBox.posZ = zPos;

	boundingBox.sizeX = xSize;
	boundingBox.sizeY = ySize;
	boundingBox.sizeZ = zSize;
}

AABB CollideMesh::getAABB()
{
	return boundingBox;
}