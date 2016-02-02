#ifndef CAPTUREMESH_H
#define CAPTUREMESH_H

#include "CollideMesh.h"

class CaptureMesh
{
private:
	AABBSingle aabb;
	std::vector<AABBSingle> subAabbs;

	vec3 pos;
	int capID;
public:
	CaptureMesh();
	CaptureMesh(vec3 pos, int capID);
	CaptureMesh(vec3 pos, int capID, AABBSingle aabb);
	CaptureMesh(CaptureLoaded aabbLoad);
	~CaptureMesh();

	void setPos(vec3 pos);
	vec3 getPos() { return pos; };

	AABBSingle getAABB() { return aabb; };
	void setAABB(AABBSingle aabb);

	std::vector<AABBSingle> getSubAABBs() { return subAabbs; };
	void setBoxes(CaptureLoaded aabbLoad);
};
#endif