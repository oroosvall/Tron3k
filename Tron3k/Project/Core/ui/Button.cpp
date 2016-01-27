#include "Button.h"

Button::Button()
{
	textureIndexList[0] = -1;
	textureIndexList[1] = -1;
	textureIndexInUse = -1;
	worldMatrix = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
					};
	uniqueKey = -1;
}
Button::Button(glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey) //Kolla mer på denna
{
	glm::vec2 posXY[4];
	glm::vec2 uv2[4];
	for (int i = 0; i < 4; i++)
	{
		posXY[i] = positions[i];
		uv2[i] = uv[i];
	}

	pos = uiVertex(posXY, uv2);

	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;
}
Button::~Button() {}

void Button::render(int id)
{
	
}

void Button::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

void Button::changeTexUsed() 
{
	if(textureIndexInUse == 1)
		textureIndexInUse = 0;
	else
		textureIndexInUse = 1;

}

void Button::scalePositions(int scale, int id) 
{
	worldMatrix[0].x = scale;
	worldMatrix[1].y = scale;
	worldMatrix[2].z = scale;
}

void Button::fromPosToQuadScreen()
{

}

int Button::checkCollision(glm::vec2 mpos)
{
	int returnValue = -1;
	if (mpos.x >= pos.vertexList[1].x && mpos.x <= pos.vertexList[2].x)
	{
		if (mpos.y >= pos.vertexList[1].y && mpos.y <= pos.vertexList[2].y)
		{
			returnValue = uniqueKey;
		}
	}
	
	return returnValue;
}

uiVertex* Button::returnPosAUv(int id)
{
	return &pos;
}