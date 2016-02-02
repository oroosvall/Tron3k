#include "Button.h"

Button::Button()
{
	uiRender = nullptr;
	pos[0] = glm::vec2(0.0f, 0.0f);
	pos[1] = glm::vec2(0.0f, 0.0f);
	textureRes[0] = glm::vec2(0.0f, 0.0f);
	textureRes[1] = glm::vec2(0.0f, 0.0f);
	center = glm::vec2(0.0f, 0.0f);

	textureIdList[0] = -1;
	textureIdList[1] = -1;
	textureIdInUse = -1;
	worldMatrix = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
					};
	uniqueKey = -1;
	hoverCheckKey = 0;
	lastScale = 1.0f;
}

Button::Button(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, int hoverKey, IRenderPipeline* uiRender, glm::vec2 textRes1, glm::vec2 textRes2)
{
	this->uiRender = uiRender;
	this->center = center;
	this->textureRes[0] = textRes1;
	this->textureRes[1] = textRes2;

	textureIdList[0] = textureId1;
	textureIdList[1] = textureId2;
	this->uniqueKey = uniqueKey;
	hoverCheckKey = hoverKey;
	textureIdInUse = textureId1;

	lastScale = 1.0f;

	float xScale = textureRes[0].x / 1980;
	float yScale = textureRes[0].y / 1080;

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

Button::~Button() {}

void Button::render(int id)
{
	uiRender->ui_renderQuad((float*)&worldMatrix[0][0], textureIdInUse, 1.0f, id);
}

void Button::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

void Button::changeTexUsed(int id) 
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

	textureIdInUse = textureIdList[id];
}

void Button::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	// setpos
	worldMatrix[0].w = positions.x;
	worldMatrix[1].w = positions.y;
	worldMatrix[2].w = 0.0f;
	pos[0] = glm::vec2(worldMatrix[0].w - worldMatrix[0].x, worldMatrix[1].w - worldMatrix[1].y);
	pos[1] = glm::vec2(worldMatrix[0].w + worldMatrix[0].x, worldMatrix[1].w + worldMatrix[1].y);

}

int Button::checkCollision(glm::vec2 mpos)
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

void Button::hoverCheck(glm::vec2 mpos)
{
	int returnValue = -1;

	if (hoverCheckKey == 1)
	{
		if (mpos.x > pos[0].x && mpos.x < pos[1].x)
		{
			if (mpos.y > pos[0].y && mpos.y < pos[1].y)
				changeTexUsed(1);
			else if (textureIdInUse != textureIdList[0])
				changeTexUsed(0);
		}
		else if (textureIdInUse != textureIdList[0])
			changeTexUsed(0);
	}

}

void Button::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < 2; i++)
	{
		textureIdList[i] = uiTextureIds[textureIdList[i]];
	}
	textureIdInUse = textureIdList[0];
}

void Button::scaleBarFromRight(float procentOfMax)
{
	//Calc the new size.
	float scale = (textureRes[0].x * procentOfMax) / 1920.0f;
	worldMatrix[0].x = scale;

	//Calc the new position
	float difference = lastScale - scale;

	difference = (difference * textureRes[0].x) / 1920.0f;
	worldMatrix[0].w += difference;

	lastScale = scale;
}