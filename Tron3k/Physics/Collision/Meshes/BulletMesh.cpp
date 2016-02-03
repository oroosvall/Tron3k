#include "BulletMesh.h"

BulletMesh::BulletMesh()
{
	pos = vec3(0, 0, 0);
	aabb.pos = pos;
	sphere.pos = pos;

	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);

	sphere.radius = 0.6f;
}

BulletMesh::BulletMesh(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
	sphere.pos = pos;

	aabb.max = pos + vec3(1, 1, 1);
	aabb.min = pos + vec3(-1, -1, -1);

	sphere.radius = 0.6f;
}

BulletMesh::BulletMesh(vec3 pos, AABBSingle aabb, Sphere sphere)
{
	setAABB(aabb);
	setSphere(sphere);

	setPos(pos);
}

BulletMesh::~BulletMesh()
{

}

void BulletMesh::setPos(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
	sphere.pos = pos;
}

void BulletMesh::setAABB(AABBSingle aabb)
{
	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	if (aabb.pos != this->pos)
		setPos(pos);
}

void BulletMesh::setSphere(Sphere sphere)
{
	this->sphere.radius = sphere.radius;

	if (sphere.pos != this->pos)
		setPos(pos);
}