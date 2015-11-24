#ifndef COLLIDEMESH_H
#define COLLIDEMESH_H

#include <vector>

//Not sure if this should be a class, but it is temporarily
struct AABB
{
	float posX, posY, posZ;
	float sizeX, sizeY, sizeZ;
};

//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:
	AABB boundingBox;
public:
	CollideMesh();
	~CollideMesh();

	void addAABB(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize);
	AABB getAABB();
};
#endif