#include "InputBox.h"

InputBox::InputBox()
{
	uiRender = nullptr;
	pos[0] = glm::vec2(0.0f);
	pos[1] = glm::vec2(0.0f);
	center = glm::vec2(0.0f);

	textureRes = glm::vec2(0.0f);;
	textureId = -1;

	uniqueKey = -1;

	worldMatrix = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };

	active = false;

	pivot = glm::vec3(0.0f);;
}
InputBox::InputBox(glm::vec2 center, int textureId1, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes)
{
	this->uiRender = uiRender;
	this->center = center;
	this->textureRes = textRes;

	textureId = textureId1;
	this->uniqueKey = uniqueKey;
	hoverCheckKey = 0;

	float xScale = textRes.x / 1980;
	float yScale = textRes.y / 1080;

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
InputBox::~InputBox()
{
}

void InputBox::render(int id)
{
	uiRender->ui_renderQuad((float*)&worldMatrix[0][0], (float*)&pivot.x, textureId, 1.0f, id);


}

void InputBox::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[0].w = x;
	worldMatrix[1].w = y;
}

void InputBox::changeTexUsed(int id)
{
}

void InputBox::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	// setpos
	worldMatrix[0].w = positions.x;
	worldMatrix[1].w = positions.y;
	worldMatrix[2].w = 0.0f;
	pos[0] = glm::vec2(worldMatrix[0].w - worldMatrix[0].x, worldMatrix[1].w - worldMatrix[1].y);
	pos[1] = glm::vec2(worldMatrix[0].w + worldMatrix[0].x, worldMatrix[1].w + worldMatrix[1].y);
}

int InputBox::checkCollision(glm::vec2 mpos)
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

void InputBox::hoverCheck(glm::vec2 mpos)
{
}

void InputBox::setTexture(std::vector<GLuint> uiTextureIds)
{
}

void InputBox::scaleBar(float procentOfMax, bool fromRight)
{
	pivot = glm::vec3(1.0f, 0.0f, 0.0f);

	float scale = (textureRes.x * procentOfMax) / 1920.0f;
	worldMatrix[0].x = scale;

	float fullLength = textureRes.x / 1920.0f;
	pivot.x = fullLength - scale;

	if (!fromRight)
		pivot.x = -pivot.x;
}

bool InputBox::activeOrNot()
{
	return active;
}