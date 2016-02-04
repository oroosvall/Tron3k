#include "WorldMesh.h"

WorldMesh::WorldMesh()
{
	pos = vec3(0, 0, 0);
	aabb.pos = pos;

	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);
}

WorldMesh::WorldMesh(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
	aabb.max = pos + vec3(1, 1, 1);
	aabb.min = pos + vec3(-1, -1, -1);
}

WorldMesh::WorldMesh(AABBloaded aabbLoad)
{
	pos = vec3(aabbLoad.pos);

	aabb.pos = vec3(aabbLoad.pos);

	aabb.max = vec3(aabbLoad.max);
	aabb.min = vec3(aabbLoad.min);

	for (int i = 0; i < aabbLoad.ObbBoxes.size(); i++)
	{
		obbs.push_back(OBB());
		obbs[i].init(&aabbLoad.ObbBoxes[i]);
	}
}

WorldMesh::WorldMesh(vec3 pos, AABBSingle aabb)
{
	setAABB(aabb);

	setPos(pos);
}

WorldMesh::~WorldMesh()
{

}

OBB* WorldMesh::getOBB(int pos)
{
	if (pos < obbs.size())
		return &obbs[pos];
	return nullptr;
}

void WorldMesh::setPos(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
}

void WorldMesh::setAABB(AABBSingle aabb)
{
	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	if (aabb.pos != this->pos)
		setPos(pos);
}

void WorldMesh::setBoxes(AABBloaded aabbLoad)
{
	obbs.clear();

	pos = vec3(aabbLoad.pos);

	aabb.pos = vec3(aabbLoad.pos);

	aabb.max = vec3(aabbLoad.max);
	aabb.min = vec3(aabbLoad.min);

	for (int i = 0; i < aabbLoad.ObbBoxes.size(); i++)
	{
		obbs.push_back(OBB());
		obbs[i].init(&aabbLoad.ObbBoxes[i]);
	}
}