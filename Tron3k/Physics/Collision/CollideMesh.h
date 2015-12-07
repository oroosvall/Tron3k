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


struct OBB
{
	glm::vec4 corners[8];
};

struct AABB
{
	glm::vec4 pos;
	glm::vec4 max;
	glm::vec4 min;

	std::vector<OBB> ObbBoxes;
};



struct Cylinder
{
	glm::vec3 pos;
	
	float radius;
	float height;
};

struct AngledCylinder
{
	glm::vec3 pos;
	glm::vec3 normal;

	float radius;
	float height;
};

struct Sphere
{
	glm::vec3 pos;

	float radius;
};

//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:
	AABB boundingBox;
	Cylinder cylinder;

	void getCylinderFromAABB();
public:
	CollideMesh();
	~CollideMesh();

	void init();

	void setAABB(glm::vec3 pos, glm::vec3 max, glm::vec3 min);
	void setAABB(AABB aabb);
	AABB getAABB();


	
	void setCylinder(glm::vec3 pos, float radius, float height);
	Cylinder getCylinder();

	void setPos(glm::vec3 pos);
};
#endif