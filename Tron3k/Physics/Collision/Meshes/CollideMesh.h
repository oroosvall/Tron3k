#pragma once
#ifndef COLLIDEMESH_H
#define COLLIDEMESH_H

#include <vector>
#include"MeshTypes.h"

using namespace glm;

//Not sure if this should be a class, but it is temporarily



//contains several levels of meshes, one that is just a cube, and then one or several below that is a simple mesh, not sure how to solve yet, might work.
class CollideMesh
{
private:

	Cylinder cylinder;
	Sphere sphere;
	AngledCylinder angledCylinder;
	void getCylinderFromAABB();
	void getSphereFromAABB();
	void getAngCylinderFromAABB();
public:
	AABB boundingBox;
	AABBCapPoint capBox; //only used for capture boxes;
	CollideMesh();
	~CollideMesh();

	void init();

	void setAABB(vec3 pos, vec3 max, vec3 min);
	void setAABB(AABB aabb);
	void setAABB(AABBloaded* aabb);
	AABB* getAABB();



	void setCylinder(vec3 pos, float radius, float height);
	Cylinder getCylinder();

	void setSphere(vec3 pos, float radius);
	void setSphere(Sphere sphere);
	Sphere getSphere();

	AABBCapPoint getCapBox() { return capBox; };

	void setPos(vec3 pos);
};
#endif