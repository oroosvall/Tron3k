#ifndef EFFECTMESH_H
#define EFFECTMESH_H

#include"../Collision/Meshes/CollideMesh.h"
#include "../Collision/Meshes/MeshTypes.h"

class EffectMesh
{
private:
	AABBSingle aabb;
	Sphere sphere;
	OBB obb;

	vec3 pos;

	unsigned int etype;
	int pID;
	int eID;

public:
	EffectMesh();
	EffectMesh(vec3 pos, AABBSingle aabb, OBB obb, unsigned int etype, int pID, int eID);
	EffectMesh(vec3 pos, AABBSingle aabb, Sphere sphere, unsigned int etype, int pID, int eID);
	~EffectMesh() {};

	void init();

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	OBB* getOBB() { return &obb; };
	void setOBB(OBB obb);

	Sphere getSphere() { return sphere; };
	void setSphere(Sphere sphere);

	vec3 getPos() { return pos; };
	void setPos(vec3 pos);

	void setIDs(unsigned int eType, int pID, int eID);
	void getIDs(unsigned int &eType, int &pID, int &eID);

	unsigned int getEType() { return etype; };
	int getEID() { return eID; };
	int getPID() { return pID; };
};
#endif