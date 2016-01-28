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

Button::Button(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes)
{
	this->uiRender = uiRender;
	this->center = center;
	this->textureRes = textRes;

	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;

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

Button::~Button() {}

void Button::render(int id)
{
	uiRender->ui_renderQuad((float*)&worldMatrix[0][0], textureIndexInUse, 1.0f, id);//&worldtest[0][0], uiTextureIds[0], 1.0f)
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

void Button::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	glm::vec2 size(0.0f,0.0f);
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

void Button::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < 2; i++)
	{
		textureIndexList[i] = uiTextureIds[textureIndexList[i]];
	}
	textureIndexInUse = textureIndexList[0];
}