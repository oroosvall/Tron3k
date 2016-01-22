#include"EffectMesh.h"

EffectMesh::EffectMesh()
{
	mesh = CollideMesh();
	etype = 0;
	pID = -1;
	eID = -1;
}

EffectMesh::EffectMesh(CollideMesh mesh, unsigned int etype, int pID, int eID)
{
	this->mesh = mesh;
	this->etype = etype;
	this->pID = pID;
	this->eID = eID;
}

void EffectMesh::setCollisionMesh(CollideMesh colMesh)
{
	mesh = colMesh;
}

void EffectMesh::setIDs(unsigned int eType, int pID, int eID)
{
	this->etype = eType;
	this->pID = pID;
	this->eID = eID;
}

void EffectMesh::getIDs(unsigned int &eType, int &pID, int &eID)
{
	eType = etype;
	pID = this->pID;
	eID = this->eID;
}