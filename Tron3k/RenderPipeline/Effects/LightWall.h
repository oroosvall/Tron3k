#ifndef LIGHTWALL_H
#define LIGHTWALL_H

#include <glm\glm.hpp>
#include <vector>

using std::vector;

struct LightWallQuad
{
	glm::vec3 pos1;
	glm::vec2 uv1;
	glm::vec3 pos2;
	glm::vec2 uv2;
};

struct PlayerWall
{
	int PID;
	vector<LightWallQuad> lightQuad;
};

class LightWall
{

private:
	
public:

	vector<PlayerWall> playerWall;

	void addQuad(int player, glm::vec3 pos1, glm::vec2 uv1, glm::vec3 pos2, glm::vec2 uv2);
	void setLastEndPos(int player, glm::vec3 pos);
	void removePlayerWall(int player);
};

#endif