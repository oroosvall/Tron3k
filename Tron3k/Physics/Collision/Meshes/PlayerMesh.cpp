#include "PlayerMesh.h"

PlayerMesh::PlayerMesh()
{
	pos = vec3(0, 0, 0);
	aabb.pos = pos;
	cylinder.pos = pos;
	sphere.pos = pos;

	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);

	cylinder.height = 1.0f;
	cylinder.radius = 1.0f;

	sphere.radius = 1.0f;
}

PlayerMesh::PlayerMesh(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
	cylinder.pos = pos;
	sphere.pos = pos;

	aabb.max = pos + vec3(1, 1, 1);
	aabb.min = pos + vec3(-1, -1, -1);

	cylinder.height = 1.0f;
	cylinder.radius = 1.0f;

	sphere.radius = 1.0f;
}

PlayerMesh::PlayerMesh(vec3 pos, AABBSingle aabb, Sphere sphere, Cylinder cylinder)
{
	setAABB(aabb);
	setSphere(sphere);
	setCylinder(cylinder);

	setPos(pos);
}

PlayerMesh::~PlayerMesh()
{

}

void PlayerMesh::setPos(vec3 pos)
{
	this->pos = pos;
	aabb.pos = pos;
	cylinder.pos = pos;
	sphere.pos = pos;
}

void PlayerMesh::setAABB(AABBSingle aabb)
{
	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	if (aabb.pos != this->pos)
		setPos(pos);
}

void PlayerMesh::setCylinder(Cylinder cylinder)
{
	this->cylinder.height = cylinder.height;
	this->cylinder.radius = cylinder.radius;

	if (cylinder.pos != this->pos)
		setPos(pos);
}

void PlayerMesh::setSphere(Sphere sphere)
{
	this->sphere.radius = sphere.radius;

	if (sphere.pos != this->pos)
		setPos(pos);
}