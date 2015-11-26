#include"CollideMesh.h"

CollideMesh::CollideMesh()
{

}

CollideMesh::~CollideMesh()
{

}

void CollideMesh::init()
{
	setAABB(0, 0, 0, 0, 0, 0);
	glm::vec3* corners = new glm::vec3[8];
	for (int i = 0; i < 8; i++)
	{
		corners[i] = glm::vec3(0, 0, 0);
	}
	setOBB(corners);
	delete corners;
}

void CollideMesh::setAABB(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize)
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

void CollideMesh::setOBB(glm::vec3* corners)
{
	for (int i = 0; i < 8; i++)
	{
		collisionBox.corners[i] = corners[i];
	}
}

OBB CollideMesh::getOBB()
{
	return collisionBox;
}

void CollideMesh::setPos(glm::vec3 pos)
{
	boundingBox.posX = pos.x;
	boundingBox.posY = pos.y;
	boundingBox.posZ = pos.z;
}

void CollideMesh::setSize(glm::vec3 size)
{
	boundingBox.sizeX = size.x;
	boundingBox.sizeY = size.y;
	boundingBox.sizeZ = size.z;
}

glm::vec3 CollideMesh::getSize()
{
	glm::vec3 size;

	size.x = boundingBox.sizeX;
	size.y = boundingBox.sizeY;
	size.z = boundingBox.sizeZ;

	return size;
}