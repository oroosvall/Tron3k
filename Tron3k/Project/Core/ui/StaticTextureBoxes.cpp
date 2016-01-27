#include "StaticTextureBoxes.h"

StaticTextureBoxes::StaticTextureBoxes()
{
	textureIndexList = -1;
	textureIndexInUse = -1;
	worldMatrix = { 1, 0, 0, 0,
				    0, 1, 0, 0,
				    0, 0, 1, 0,
				    0, 0, 0, 1 };
	uniqueKey = -1;
}
StaticTextureBoxes::StaticTextureBoxes(glm::vec2 positions[], glm::vec2 uv[], int textureId1, IRenderPipeline* uiRender)
{
	this->uiRender = uiRender;
	glm::vec2 posXY[4];
	glm::vec2 uv2[4];
	for (int i = 0; i < 4; i++)
	{
		posXY[i] = positions[i];
		uv2[i] = uv[i];
	}

	pos = uiVertex(posXY, uv2);

	textureIndexList = textureId1;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;
}
StaticTextureBoxes::~StaticTextureBoxes() {}

void StaticTextureBoxes::render(int id)
{
	//renderPipe->ui_renderQuad(&worldtest[0][0], uiTextureIds[0], 1.0f)
}

void StaticTextureBoxes::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

void StaticTextureBoxes::changeTexUsed()
{
}

void StaticTextureBoxes::scalePositions(int scale, int id)
{
	worldMatrix[0].x = scale;
	worldMatrix[1].y = scale;
	worldMatrix[2].z = scale;
}

void StaticTextureBoxes::fromPosToQuadScreen(glm::vec2 positions, int id)
{

}

int StaticTextureBoxes::checkCollision(glm::vec2 mpos)
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