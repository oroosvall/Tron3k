#ifndef EFFECTMESH_H
#define EFFECTMESH_H

#include"../Collision/CollideMesh.h"

class EffectMesh
{
private:
	CollideMesh mesh;
	unsigned int etype;
	int pID;
	int eID;

public:
	EffectMesh();
	EffectMesh(CollideMesh mesh, unsigned int etype, int pID, int eID);
	~EffectMesh() {};

	void init();

	void setCollisionMesh(CollideMesh colMesh);
	CollideMesh getCollisionMesh() { return mesh; };

	void setIDs(unsigned int eType, int pID, int eID);
	void getIDs(unsigned int &eType, int &pID, int &eID);

	unsigned int getEType() { return etype; };
};
#endif