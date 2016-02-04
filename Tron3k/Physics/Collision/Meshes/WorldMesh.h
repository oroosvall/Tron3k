#ifndef WORLDMESH_H
#define WORLDMESH_H

#include"MeshTypes.h"

class WorldMesh
{
private:
	AABBSingle aabb;
	std::vector<OBB> obbs;

	vec3 pos;
public:
	WorldMesh();
	WorldMesh(vec3 pos);
	WorldMesh(vec3 pos, AABBSingle aabb);
	WorldMesh(AABBloaded aabbLoad);
	~WorldMesh();

	void setPos(vec3 pos);
	vec3 getPos() { return pos; };

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	std::vector<OBB> getOBBs() { return obbs; };
	int getOBBSize() { return obbs.size(); };
	OBB* getOBB(int pos);
	void setBoxes(AABBloaded aabbLoad);
};
#endif