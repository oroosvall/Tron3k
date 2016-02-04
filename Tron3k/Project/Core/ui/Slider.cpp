#include "Slider.h"

Slider::Slider()
{
	uiRender = nullptr;
	pos[0] = glm::vec2(0.0f, 0.0f);
	pos[1] = glm::vec2(0.0f, 0.0f);
	pos[2] = glm::vec2(0.0f, 0.0f);
	pos[3] = glm::vec2(0.0f, 0.0f);
	textureRes[0] = glm::vec2(0.0f, 0.0f);
	textureRes[1] = glm::vec2(0.0f, 0.0f);
	center[0] = glm::vec2(0.0f, 0.0f);
	center[1] = glm::vec2(0.0f, 0.0f);

	textureIndexList[0] = -1;
	textureIndexList[1] = -1;
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

	winX = 0;
	winY = 0;
}
Slider::Slider(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, int objId1, int objId2, IRenderPipeline* uiRender, glm::vec2 textRes, glm::vec2 textRes2)
{
	this->uiRender = uiRender;
	this->center[0] = center;
	this->textureRes[0] = textRes;
	this->textureRes[1] = textRes2;
	
	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;

	objId[0] = objId1;
	objId[1] = objId2;

	winX = 0;
	winY = 0;

	float xScale = textRes.x / 1980;
	float yScale = textRes.y / 1080;

	// setpos
	worldMatrix[0][0].w = center.x;
	worldMatrix[0][1].w = center.y;
	worldMatrix[0][2].w = 0.0f;
	// set scale
	worldMatrix[0][0].x = xScale;
	worldMatrix[0][1].y = yScale;

	pos[0] = glm::vec2(worldMatrix[0][0].w - worldMatrix[0][0].x, worldMatrix[0][1].w - worldMatrix[0][1].y);
	pos[1] = glm::vec2(worldMatrix[0][0].w + worldMatrix[0][0].x, worldMatrix[0][1].w + worldMatrix[0][1].y);


	createAdditionalPoint();
}
Slider::~Slider() {}


void Slider::createAdditionalPoint()
{

}

void Slider::render(int id)
{
	for(int i = 0; i < 2; i++)
		uiRender->ui_renderQuad((float*)&worldMatrix[i][0][0], (float*)&pivot.x, textureIndexList[i], 1.0f, id);
}

void Slider::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[id][0].w = x;
	worldMatrix[id][1].w = y;
}

//Empty
void Slider::changeTexUsed(int id)
{
}

void Slider::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	// setpos
	worldMatrix[id][0].w = positions.x;
	worldMatrix[id][1].w = positions.y;
	worldMatrix[id][2].w = 0.0f;
	pos[0 + 2 * id] = glm::vec2(worldMatrix[id][0].w - worldMatrix[id][0].x, worldMatrix[id][1].w - worldMatrix[id][1].y);
	pos[1 + 2 * id] = glm::vec2(worldMatrix[id][0].w + worldMatrix[id][0].x, worldMatrix[id][1].w + worldMatrix[id][1].y);

}

int Slider::checkCollision(glm::vec2 mpos)
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

	return returnValue;
}

void Slider::hoverCheck(glm::vec2 pos)
{

}

void Slider::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < 2; i++)
	{
		textureIndexList[i] = uiTextureIds[textureIndexList[i]];
	}
}

void Slider::scaleBar(float procentOfMax, bool fromRight) //
{
	pivot = glm::vec3(1.0f, 0.0f, 0.0f);

	float scale = (textureRes[0].x * procentOfMax) / 1920.0f;
	worldMatrix[0][0].x = scale;

	float fullLength = textureRes[0].x / 1920.0f;
	pivot.x = fullLength - scale;

	if (!fromRight)
		pivot.x = -pivot.x;
}

void Slider::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;
}

void Slider::setText(std::string text)
{

}