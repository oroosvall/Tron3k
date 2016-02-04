#ifndef PLAYERMESH_H
#define PLAYERMESH_H

#include "MeshTypes.h"

class PlayerMesh
{
private:
	AABBSingle aabb;
	Cylinder cylinder;
	Sphere sphere;

	vec3 pos;
	vec3 worldSize;//encapsules collision sphere
	vec3 playerSize;//actual size used to measure against bullets
public:
	PlayerMesh();
	PlayerMesh(vec3 pos);
	PlayerMesh(vec3 pos, AABBSingle aabb, Sphere sphere, Cylinder cylinder);
	~PlayerMesh();

	void setPos(vec3 pos);
	vec3 getPos() { return pos; };

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	Cylinder getCylinder() { return cylinder; };
	void setCylinder(Cylinder cylinder);

	vec3 getWorldSize() { return worldSize; };
	void setWorldSize(vec3 size) { worldSize = size;  };
	void setWorldSize() { aabb.max = pos + worldSize; aabb.min = pos - worldSize; };

	vec3 getPlayerSize() { return playerSize; };
	void setPlayerSize(vec3 size) { playerSize = size; aabb.max = pos + playerSize; aabb.min = pos - playerSize; };
	void setPlayerSize() { aabb.max = pos + playerSize; aabb.min = pos - playerSize; };

	Sphere getSphere() { return sphere; };
	void setSphere(Sphere sphere);
};
#endif