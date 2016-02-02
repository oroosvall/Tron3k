#ifndef PLAYERMESH_H
#define PLAYERMESH_H

#include "CollideMesh.h"

class PlayerMesh
{
private:
	AABBSingle aabb;
	Cylinder cylinder;
	Sphere sphere;

	vec3 pos;
public:
	PlayerMesh();
	PlayerMesh(vec3 pos);
	PlayerMesh(vec3 pos, AABBSingle aabb, Sphere sphere, Cylinder cylinder);
	~PlayerMesh();

	void setPos(vec3 pos);
	vec3 getPos() { return pos; };

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	Cylinder getCylinder() { return cylinder; };
	void setCylinder(Cylinder cylinder);

	Sphere getSphere() { return sphere; };
	void setSphere(Sphere sphere);
};
#endif