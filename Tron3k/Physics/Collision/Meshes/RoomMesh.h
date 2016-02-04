#ifndef ROOMMESH_H
#define ROOMMESH_H

#include"MeshTypes.h"
#include "WorldMesh.h"

class RoomMesh
{
private:
	int roomID;
	AABBSingle aabb;

	std::vector<WorldMesh> roomBoxes;
public:
	RoomMesh();
	RoomMesh(vec3 pos, int roomID);
	RoomMesh(AABBSingle aabb, int roomID, std::vector<WorldMesh> roomBoxes);
	RoomMesh(std::vector<WorldMesh> roomBoxes);
	~RoomMesh();

	AABBSingle* getAABB() { return &aabb; };
	void setAABB(AABBSingle aabb);

	void setRoomID(int roomID);
	int getRoomID() { return roomID; };

	std::vector<WorldMesh>* getRoomBoxes() { return &roomBoxes; };
	WorldMesh* getSpecificBox(int pos);

	void setBoxes(std::vector<WorldMesh> roomBoxes);
	void addBox(WorldMesh roomBox);
};
#endif/