#ifndef COLLIDEMESH_H
#define COLLIDEMESH_H

#include <vector>
#include"../../Dependencies/glm/glm/glm.hpp"
//Not sure if this should be a class, but it is temporarily

//corner defines
#define DLF 0
#define DRF 1
#define ULF 2
#define URF 3
#define DLB 4
#define DRB 5
#define ULB 6
#define URB 7

struct AABB
{
	float posX, posY, posZ;
	float sizeX, sizeY, sizeZ;
};

struct OBB
{
	glm::vec3 pos;
	glm::vec3 corners[8];

};

struct Cylinder
{
	glm::vec3 pos;
	
	float radius;
	float height;
};

//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:
	AABB boundingBox;
	OBB collisionBox;
	Cylinder cylinder;
public:
	CollideMesh();
	~CollideMesh();

	void init();

	void setAABB(float xPos, float yPos, float zPos, float xSize, float ySize, float zSize);
	AABB getAABB();

	void setOBB(glm::vec3 pos, glm::vec3* corners);
	OBB getOBB();
	
	void setCylinder(glm::vec3 pos, float radius, float height);
	Cylinder getCylinder();

	void setPos(glm::vec3 pos);

	void setSize(glm::vec3 size);
	glm::vec3 getSize();
};
#endif