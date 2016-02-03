#include"EffectMesh.h"

EffectMesh::EffectMesh()
{
	init();
}

EffectMesh::EffectMesh(vec3 pos, AABBSingle aabb, OBB obb, unsigned int etype, int pID, int eID)
{
	this->pos = pos;
	this->aabb = aabb;
	this->obb = obb;
	this->etype = etype;
	this->pID = pID;
	this->eID = eID;
}

EffectMesh::EffectMesh(vec3 pos, AABBSingle aabb, Sphere sphere, unsigned int etype, int pID, int eID)
{
	this->pos = pos;
	this->aabb = aabb;
	this->sphere = sphere;
	this->etype = etype;
	this->pID = pID;
	this->eID = eID;
}

void EffectMesh::init()
{
	pos = vec3(0, 0, 0);
	aabb.pos = pos;
	sphere.pos = pos;
	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);
	etype = 0;
	pID = -1;
	eID = -1;
}

void EffectMesh::setAABB(AABBSingle aabb)
{
	this->aabb = aabb;
}

void EffectMesh::setOBB(OBB obb)
{
	this->obb = obb;
}

void EffectMesh::setSphere(Sphere sphere)
{
	this->sphere = sphere;
}

void EffectMesh::setPos(vec3 pos)
{
	this->pos = pos;
	sphere.pos = pos;
	aabb.pos = pos;
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