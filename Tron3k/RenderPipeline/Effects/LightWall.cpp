#include "LightWall.h"

void LightWall::addQuad(int player, glm::vec3 pos1, glm::vec2 uv1, glm::vec3 pos2, glm::vec2 uv2)
{
	bool found = false;
	for (size_t i = 0; i < playerWall.size(); i++)
	{
		if (playerWall[i].PID == player)
		{
			found = true;
			player = i;
			break;
		}
	}

	if (found == false)
	{
		playerWall.push_back(PlayerWall());
		playerWall[playerWall.size() - 1].PID = player;
		player = playerWall.size()-1;
	}

	playerWall[player].lightQuad.push_back(LightWallQuad());
	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].pos1 = pos1;
	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].pos2 = pos2;
	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].uv1 = uv1;
	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].uv2 = uv2;

}

void LightWall::setLastEndPos(int player, glm::vec3 pos)
{
	for (size_t i = 0; i < playerWall.size(); i++)
	{
		if (playerWall[i].PID == player)
		{
			player = i;
			break;
		}
	}

	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].pos2 = pos;

	float distance = glm::distance(playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].pos2, playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].pos1);

	playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].uv2.x = playerWall[player].lightQuad[playerWall[player].lightQuad.size() - 1].uv1.x + distance / 2.0f;

}

void LightWall::removePlayerWall(int player)
{
	for (size_t i = 0; i < playerWall.size(); i++)
	{
		if (playerWall[i].PID == player)
		{
			player = i;
			break;
		}
	}

	playerWall.erase(playerWall.begin() + player);

}