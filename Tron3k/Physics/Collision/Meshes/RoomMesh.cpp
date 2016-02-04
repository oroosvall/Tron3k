#include "RoomMesh.h"

RoomMesh::RoomMesh()
{
	aabb.pos = vec3(0, 0, 0);

	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);
}

RoomMesh::RoomMesh(vec3 pos, int roomID)
{
	aabb.pos = pos;
	aabb.max = pos + vec3(1, 1, 1);
	aabb.min = pos + vec3(-1, -1, -1);
}

RoomMesh::RoomMesh(AABBSingle aabb, int roomID, std::vector<WorldMesh> roomBoxes)
{

	this->aabb.pos = aabb.pos;

	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	this->roomID = roomID;

	this->roomBoxes = roomBoxes;
}

RoomMesh::RoomMesh(std::vector<WorldMesh> roomBoxes)
{
	aabb.pos = vec3(0, 0, 0);
	aabb.max = vec3(1, 1, 1);
	aabb.min = vec3(-1, -1, -1);
	
	roomID = -1;

	this->roomBoxes = roomBoxes;
}

RoomMesh::~RoomMesh()
{

}

WorldMesh* RoomMesh::getSpecificBox(int pos)
{
	if (pos < roomBoxes.size())
		return &roomBoxes[pos];
	return nullptr;
}

void RoomMesh::setAABB(AABBSingle aabb)
{
	this->aabb.max = aabb.max;
	this->aabb.min = aabb.min;

	this->aabb.pos = aabb.pos;
}

void RoomMesh::setRoomID(int roomID)
{
	this->roomID = roomID;
}

void RoomMesh::setBoxes(std::vector<WorldMesh> roomBoxes)
{
	/*Lägg in kod som liknar den här typ, kopiera över innehållet i vektorn istället för att assigna samma vektor
	ifall ifall, kostar lite mer overhead, men hey. Överäg statisk allokering/länkade listor*/
	/*int size = in->ObbBoxes.size();

	for (int n = 0; n < size; n++)
	{
		ObbBoxes.push_back(OBB());
		ObbBoxes[n].init(&in->ObbBoxes[n]);
	}*/
}

void RoomMesh::addBox(WorldMesh roomBox)
{
	roomBoxes.push_back(roomBox);
}