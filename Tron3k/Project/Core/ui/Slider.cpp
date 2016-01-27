#include "Slider.h"

Slider::Slider()
{
	textureIndexList[0] = -1;
	textureIndexList[1] = -1;
	textureIndexInUse = -1;
	worldMatrix[0] = { 1, 0, 0, 0,
					   0, 1, 0, 0,
					   0, 0, 1, 0,
					   0, 0, 0, 1 };
	worldMatrix[1] = { 1, 0, 0, 0,
					   0, 1, 0, 0,
					   0, 0, 1, 0,
					   0, 0, 0, 1 };
	uniqueKey = -1;
	nrOfButtons = 2;
	objId[0] = -1;
	objId[1] = -1;
}
Slider::Slider(glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey, int objId1, int objId2) //Kolla mer på denna
{
	glm::vec2 posXY[4];
	glm::vec2 uv2[4];
	for (int i = 0; i < 4; i++)
	{
		posXY[i] = positions[i];
		uv2[i] = uv[i];
	}

	pos[0] = uiVertex(posXY, uv2);

	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;

	objId[0] = objId1;
	objId[1] = objId2;
}
Slider::~Slider() {}

void Slider::render(int id)
{

}

void Slider::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[id][0].w = x;
	worldMatrix[id][1].w = y;
}

void Slider::changeTexUsed()
{
}

void Slider::scalePositions(int scale, int id)
{
	worldMatrix[id][0].x = scale;
	worldMatrix[id][1].y = scale;
	worldMatrix[id][2].z = scale;
}

void Slider::fromPosToQuadScreen()
{

}

int Slider::checkCollision(glm::vec2 mpos)
{
	int returnValue = -1;
	if (mpos.x >= pos[0].vertexList[1].x && mpos.x <= pos[0].vertexList[2].x)
	{
		if (mpos.y >= pos[0].vertexList[1].y && mpos.y <= pos[0].vertexList[2].y)
		{
			returnValue = uniqueKey;
		}
	}

	return returnValue;
}

uiVertex* Slider::returnPosAUv(int id)
{
	return pos;
}






























