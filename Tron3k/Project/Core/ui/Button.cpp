#include "Button.h"

Button::Button()
{
	textureIndexList[0] = -1;
	textureIndexList[1] = -1;
	textureIndexInUse = -1;
	objId = 0;
	worldMatrix = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
					};
	uniqueKey = -1;
}
Button::Button(glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int objId, int uniqueKey)
{
	for (int i = 0; i < 4; i++)
		pos[i] = Vertex(positions[i].x, positions[i].y, uv[i].x, uv[i].y);
	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->objId = objId;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;
}
Button::~Button() {}

void Button::setWorldMatrix(float x, float y) 
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

glm::mat4 Button::returnWorldMatrix() 
{
	return worldMatrix;
}

Vertex* Button::returnPosAUv()
{
	return pos;
}

int Button::returnObjId()
{
	return objId;
}

int Button::changeTexUsed() 
{
	if(textureIndexInUse == textureIndexList[0])
		textureIndexInUse = textureIndexList[1];
	else
		textureIndexInUse = textureIndexList[0];

	return textureIndexInUse;
}

void Button::scalePositions(int scale) 
{
	worldMatrix[0].x = scale;
	worldMatrix[1].y = scale;
	worldMatrix[2].z = scale;
}

int Button::checkCollision(glm::vec2 mpos)
{
	int returnValue = -1;
	if (mpos.x >= pos[1].x && mpos.x <= pos[2].x)
	{
		if (mpos.y >= pos[1].y && mpos.y <= pos[2].y)
		{
			returnValue = uniqueKey;
		}
	}

	return returnValue;
}