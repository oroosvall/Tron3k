#ifndef COLLIDEMESH_H
#define COLLIDEMESH_H

#include <vector>
#include"../../Dependencies/glm/glm/glm.hpp"
//Not sure if this should be a class, but it is temporarily
struct AABB
{
	float posX, posY, posZ;
	float sizeX, sizeY, sizeZ;
};

struct OBB
{
	glm::vec3 corners[4];

};

//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:
	AABB boundingBox;
	OBB CollisionBox;
public:
	CollideMesh();
	~CollideMesh();

	void setAABB(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize);
	AABB getAABB();

	void setOBB(glm::vec3* corners);
	OBB getOBB();
};
#endif