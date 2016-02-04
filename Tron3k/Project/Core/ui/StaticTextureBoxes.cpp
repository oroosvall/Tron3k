#include "StaticTextureBoxes.h"

StaticTextureBoxes::StaticTextureBoxes()
{
	uiRender = nullptr;
	center = glm::vec2(0.0f, 0.0f);
	textureInUse = -1;
	worldMatrix = { 1, 0, 0, 0,
				    0, 1, 0, 0,
				    0, 0, 1, 0,
				    0, 0, 0, 1 };
	uniqueKey = -1;
	nrOfTextures = 0;

	winX = 0;
	winY = 0;
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
		this->textureRes[i] = textRes[textureId1[i]];
	}
	textureInUse = textureIndexList[0];
	uniqueKey = -1;

	winX = 0;
	winY = 0;

	float xScale = textureRes[0].x / 1980;
	float yScale = textureRes[0].y / 1080;

	// setpos
	worldMatrix[0].w = center.x;
	worldMatrix[1].w = center.y;
	worldMatrix[2].w = 0.0f;
	// set scale
	worldMatrix[0].x = xScale;
	worldMatrix[1].y = yScale;

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
	uiRender->ui_renderQuad((float*)&worldMatrix[0][0], (float*)&pivot.x, textureInUse, 1.0f, id);
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

	textureInUse = textureIndexList[id];
}

void StaticTextureBoxes::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	// setpos
	worldMatrix[0].w = positions.x;
	worldMatrix[1].w = positions.y;
	worldMatrix[2].w = 0.0f;
}

int StaticTextureBoxes::checkCollision(glm::vec2 mpos)
{
	//int returnValue = -1;
	//if (mpos.x > pos[0].x && mpos.x < pos[1].x)
	//{
	//	if (mpos.y > pos[0].y && mpos.y < pos[1].y)
	//	{
	//		returnValue = uniqueKey;
	//	}
	//}
	//
	//return returnValue;
	return -1;
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

void StaticTextureBoxes::scaleBar(float procentOfMax, bool fromRight) //
{
	pivot = glm::vec3(1.0f, 0.0f, 0.0f);

	float scale = (textureRes[0].x * procentOfMax) / 1920.0f;
	worldMatrix[0].x = scale;

	float fullLength = textureRes[0].x / 1920.0f;
	pivot.x = fullLength - scale;

	if (!fromRight)
		pivot.x = -pivot.x;
}

void StaticTextureBoxes::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;
}

void StaticTextureBoxes::setText(std::string text)
{

}