#include "CaptureMesh.h"

CaptureMesh::CaptureMesh()
{
	pos = vec3(0, 0, 0);
	aabb.pos = pos;

	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);

	capID = -1;
}

CaptureMesh::CaptureMesh(vec3 pos, int capID)
{
	this->pos = pos;
	aabb.pos = pos;
	aabb.max = pos + vec3(1, 1, 1);
	aabb.min = pos + vec3(-1, -1, -1);

	this->capID = capID;
}

CaptureMesh::CaptureMesh(CaptureLoaded aabbLoad)
{
	capID = aabbLoad.roomID;

	pos = vec3(aabbLoad.bigAABB.pos);

	aabb.pos = vec3(aabbLoad.bigAABB.pos);

	aabb.max = vec3(aabbLoad.bigAABB.max);
	aabb.min = vec3(aabbLoad.bigAABB.min);

	for (int i = 0; i < aabbLoad.subcount; i++)
	{
		subAabbs.push_back(AABBSingle());
		subAabbs[i].pos = vec3(aabbLoad.subabbs[i].pos);
		subAabbs[i].max = vec3(aabbLoad.subabbs[i].max);
		subAabbs[i].min = vec3(aabbLoad.subabbs[i].min);
	}
}

CaptureMesh::CaptureMesh(vec3 pos, int capID, AABBSingle aabb)
{
	setAABB(aabb);
	
	this->capID = capID;

	setPos(pos);
}

CaptureMesh::~CaptureMesh()
{

}

void CaptureMesh::setPos(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
}

void CaptureMesh::setCapID(int capID)
{
	this->capID = capID;
}

void CaptureMesh::setAABB(AABBSingle aabb)
{
	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	if (aabb.pos != this->pos)
		setPos(pos);
}

void CaptureMesh::setBoxes(CaptureLoaded aabbLoad)
{
	subAabbs.clear();
	capID = aabbLoad.roomID;

	pos = vec3(aabbLoad.bigAABB.pos);

	aabb.pos = vec3(aabbLoad.bigAABB.pos);

	aabb.max = vec3(aabbLoad.bigAABB.max);
	aabb.min = vec3(aabbLoad.bigAABB.min);

	for (int i = 0; i < aabbLoad.subcount; i++)
	{
		subAabbs.push_back(AABBSingle());
		subAabbs[i].pos = vec3(aabbLoad.subabbs[i].pos);
		subAabbs[i].max = vec3(aabbLoad.subabbs[i].max);
		subAabbs[i].min = vec3(aabbLoad.subabbs[i].min);
	}
}