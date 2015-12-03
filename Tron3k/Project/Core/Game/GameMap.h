#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include <glm\glm.hpp>

using std::vector;

struct OBB
{
	glm::vec4 point[8];
	glm::mat4 transform;
};

struct ABB
{
	glm::vec4 pos[3];
	vector<OBB> obbBoxes;
};

struct Chunk
{
	vector<ABB> abbBoxes;
	
	glm::vec3 color;

};

struct SpawPoint
{
	glm::vec3 position;
	glm::vec3 direction;

	int roomID;
	int teamID; // 0 == FFA, 1 == team1, 2 == team2
};

class GameMap
{

private:

	vector<Chunk> chunks;
	vector<SpawPoint> spawns;

	vector<int> capPoints;

public:


};


#endif