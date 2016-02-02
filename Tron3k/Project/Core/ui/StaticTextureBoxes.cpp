#include "StaticTextureBoxes.h"

StaticTextureBoxes::StaticTextureBoxes()
{
	uiRender = nullptr;
	pos[0] = glm::vec2(0.0f, 0.0f);
	pos[1] = glm::vec2(0.0f, 0.0f);
	center = glm::vec2(0.0f, 0.0f);
	textureInUse = -1;
	worldMatrix = { 1, 0, 0, 0,
				    0, 1, 0, 0,
				    0, 0, 1, 0,
				    0, 0, 0, 1 };
	uniqueKey = -1;
	nrOfTextures = 0;
}
StaticTextureBoxes::StaticTextureBoxes(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2>  textRes)
{
	this->uiRender = uiRender;
	this->center = center;
	textureRes = new glm::vec2[nrOfTextures];
	textureIndexList = new int[nrOfTextures];

	this->nrOfTextures = nrOfTextures;
	for (int i = 0; i < nrOfTextures; i++)
	{
		textureIndexList[i] = textureId1[i];
		this->textureRes[i] = textRes[i];
	}
	textureInUse = textureIndexList[0];

	float xScale = textRes[0].x / 1980;
	float yScale = textRes[0].y / 1080;

	// setpos
	worldMatrix[0].w = center.x;
	worldMatrix[1].w = center.y;
	worldMatrix[2].w = 0.0f;
	// set scale
	worldMatrix[0].x = xScale;
	worldMatrix[1].y = yScale;

	pos[0] = glm::vec2(worldMatrix[0].w - worldMatrix[0].x, worldMatrix[1].w - worldMatrix[1].y);
	pos[1] = glm::vec2(worldMatrix[0].w + worldMatrix[0].x, worldMatrix[1].w + worldMatrix[1].y);
}
StaticTextureBoxes::~StaticTextureBoxes() 
{
	if(textureRes != nullptr)
		delete[] textureRes;
	if (textureIndexList != nullptr)
		delete[] textureIndexList;
}

void StaticTextureBoxes::render(int id)
{
	uiRender->ui_renderQuad((float*)&worldMatrix[0][0], textureInUse, 1.0f, id);
}

void StaticTextureBoxes::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

void StaticTextureBoxes::changeTexUsed(int id)
{
	float xScale = textureRes[id].x / 1980;
	float yScale = textureRes[id].y / 1080;

	// setpos
	worldMatrix[0].w = center.x;
	worldMatrix[1].w = center.y;
	worldMatrix[2].w = 0.0f;
	// set scale
	worldMatrix[0].x = xScale;
	worldMatrix[1].y = yScale;

	pos[0] = glm::vec2(worldMatrix[0].w - worldMatrix[0].x, worldMatrix[1].w - worldMatrix[1].y);
	pos[1] = glm::vec2(worldMatrix[0].w + worldMatrix[0].x, worldMatrix[1].w + worldMatrix[1].y);

	textureInUse = textureIndexList[id];
}

void StaticTextureBoxes::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	// setpos
	worldMatrix[0].w = positions.x;
	worldMatrix[1].w = positions.y;
	worldMatrix[2].w = 0.0f;
	pos[0] = glm::vec2(worldMatrix[0].w - worldMatrix[0].x, worldMatrix[1].w - worldMatrix[1].y);
	pos[1] = glm::vec2(worldMatrix[0].w + worldMatrix[0].x, worldMatrix[1].w + worldMatrix[1].y);
}

int StaticTextureBoxes::checkCollision(glm::vec2 mpos)
{
	int returnValue = -1;
	if (mpos.x > pos[0].x && mpos.x < pos[1].x)
	{
		if (mpos.y > pos[0].y && mpos.y < pos[1].y)
		{
			returnValue = uniqueKey;
		}
	}

	return returnValue;
}

void StaticTextureBoxes::hoverCheck(glm::vec2 pos)
{

}

void StaticTextureBoxes::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < nrOfTextures; i++)
	{
		textureIndexList[i] = uiTextureIds[textureIndexList[i]];
	}
	textureInUse = textureIndexList[0];
}

