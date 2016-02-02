#ifndef BULLETMESH_H
#define BULLETMESH_H

#include "CollideMesh.h"

class BulletMesh
{
private:
	AABBSingle aabb;
	Sphere sphere;

	vec3 pos;
public:
	BulletMesh();
	BulletMesh(vec3 pos);
	BulletMesh(vec3 pos, AABBSingle aabb, Sphere sphere);
	~BulletMesh();

	void setPos(vec3 pos);
	vec3 getPos() { return pos; };

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	Sphere getSphere() { return sphere; };
	void setSphere(Sphere sphere);
};
#endif