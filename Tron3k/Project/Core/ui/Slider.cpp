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

	winX = 0;
	winY = 0;
	startWMX = 0;
	startWMY = 0;

	pivot = glm::vec3(0.0f);
}
Slider::Slider(glm::vec2 center, int textureId1, int textureId2, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes, glm::vec2 textRes2)
{
	this->uiRender = uiRender;
	this->center[0] = center;
	this->textureRes[0] = textRes;
	this->textureRes[1] = textRes2;
	
	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;

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

	startWMX = worldMatrix[0][0].w;
	startWMY = worldMatrix[0][1].w;

	pivot = glm::vec3(0.0f);

	createAdditionalPoint();
}
Slider::~Slider() { uiRender = nullptr; }


void Slider::createAdditionalPoint()
{
	center[1] = center[0];
	
	float xScale = textureRes[1].x / 1980;
	float yScale = textureRes[1].y / 1080;

	// setpos
	worldMatrix[1][0].w = center[1].x;
	worldMatrix[1][1].w = center[1].y;
	worldMatrix[1][2].w = 0.0f;
	// set scale
	worldMatrix[1][0].x = xScale;
	worldMatrix[1][1].y = yScale;

	pos[2] = glm::vec2(worldMatrix[1][0].w - worldMatrix[1][0].x, worldMatrix[1][1].w - worldMatrix[1][1].y);
	pos[3] = glm::vec2(worldMatrix[1][0].w + worldMatrix[1][0].x, worldMatrix[1][1].w + worldMatrix[1][1].y);
}


void Slider::renderQuad(int id)
{
	for(int i = 0; i < 2; i++)
		uiRender->ui_renderQuad((float*)&worldMatrix[i][0][0], (float*)&pivot.x, textureIndexList[i], 1.0f, id);
}

void Slider::setWorldMatrix(int id, float x, float y)
{
	if(x >= pos[0].x && x <= pos[1].x)
		worldMatrix[id][0].w = x;
	//worldMatrix[id][1].w = y;
}

void Slider::resetWorldMatrix(int id)
{
	worldMatrix[id][0].w = startWMX;
	worldMatrix[id][1].w = startWMY;
}

float Slider::getWorldX()
{
	return worldMatrix[1][0].w;
}

int Slider::checkCollision(glm::vec2 mpos, float &newSoundProcent, float &xPos)
{
	int returnValue = -1;
	if (mpos.x > pos[0].x && mpos.x < pos[1].x)
	{
		if (mpos.y > pos[0].y && mpos.y < pos[1].y)
		{
			returnValue = uniqueKey;
			setWorldMatrix(1, mpos.x, mpos.y);
			newSoundProcent = calculateSoundProcent(mpos.x);
			xPos = mpos.x;
		}
	}

	return returnValue;
}

void Slider::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < 2; i++)
	{
		textureIndexList[i] = uiTextureIds[textureIndexList[i]];
	}
}

void Slider::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;
}

float Slider::calculateSoundProcent(float mposX)
{
	float left = pos[0].x;
	float right = pos[1].x;
	float current = mposX;

	if (left < 0)
		left *= -1.0f;
	right += left;
	current += left;
	left -= left;
	right *= 2.5f;
	current *= 2.5;

	return current / right;
}

void Slider::renderText(int id)
{

}
//Empty
void Slider::changeTexUsed(int id, int wmID)
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

void Slider::hoverCheck(glm::vec2 pos)
{

}

void Slider::scaleBar(int id, float procentOfMax, bool fromRight) //
{
}

void Slider::setText(std::string text)
{

}

std::string Slider::getText()
{
	return "";
}
void Slider::removeLastInput()
{
}
void Slider::cleanText()
{

}