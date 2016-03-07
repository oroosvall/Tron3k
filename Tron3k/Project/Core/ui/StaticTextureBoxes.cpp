#include "StaticTextureBoxes.h"

StaticTextureBoxes::StaticTextureBoxes()
{
	uiRender = nullptr;
	textureRes = nullptr;
	textureIndexList = nullptr;
	center = glm::vec2(0.0f, 0.0f);
	textureInUse = -1;
	originalWM = { 1, 0, 0, 0,
				    0, 1, 0, 0,
				    0, 0, 1, 0,
				    0, 0, 0, 1 };
	worldMatrix.push_back(originalWM);
	uniqueKey = -1;
	nrOfTextures = 0;

	winX = 0;
	winY = 0;
	startWMX = 0;
	startWMY = 0;
	show = true;
}
StaticTextureBoxes::StaticTextureBoxes(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2>  textRes)
{
	originalWM = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	worldMatrix.push_back(originalWM);

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
	worldMatrix[0][0].w = center.x;
	worldMatrix[0][1].w = center.y;
	worldMatrix[0][2].w = 0.0f;
	// set scale
	worldMatrix[0][0].x = xScale;
	worldMatrix[0][1].y = yScale;

	//backup
	startWMX = worldMatrix[0][0].w;
	startWMY = worldMatrix[0][1].w;

	//Start WM for the objects.
	originalWM = worldMatrix[0];

	show = true;
}
StaticTextureBoxes::~StaticTextureBoxes() 
{
	if(textureRes != nullptr)
		delete[] textureRes;
	if (textureIndexList != nullptr)
		delete[] textureIndexList;
	worldMatrix.clear();
}

void StaticTextureBoxes::renderText(int id)
{

}

void StaticTextureBoxes::renderQuad(int id)
{
	//uiRender->ui_renderQuad((float*)&worldMatrix[0][0], (float*)&pivot.x, textureInUse, 1.0f, id);
	if(show)
		for (int i = 0; i < worldMatrix.size(); i++)
			uiRender->ui_renderQuad((float*)&worldMatrix[i][0], (float*)&pivot.x, textureInUse, 1.0f, id);
}

void StaticTextureBoxes::setWorldMatrix(int id, float x, float y)
{
	if (id > -1 && id < worldMatrix.size())
	{
		worldMatrix[id][0].w += x;
		worldMatrix[id][1].w += y;
	}
}

void StaticTextureBoxes::resetWorldMatrix(int id)
{
	if (id > -1 && id < worldMatrix.size())
	{
		worldMatrix[id][0].w = startWMX;
		worldMatrix[id][1].w = startWMY;
	}
}

//We create a new WM since we render the same object multiple times but with different WM so it looks like different objects.
//Sub and add numbers poping up on screen for score and tokens uses this.
int StaticTextureBoxes::addNewWM()
{
	//Create a new WM
	worldMatrix.push_back(originalWM);
	//Return the new WMs index
	return worldMatrix.size() - 1; 
}

//We need to delete them since they only exists for a few seconds
void StaticTextureBoxes::deleteOldestWM()
{
	//Delete the oldest wm since it goes away first
	int tmpSize = worldMatrix.size();
	if(tmpSize > 0)
		worldMatrix.erase(worldMatrix.begin());
}

void StaticTextureBoxes::changeTexUsed(int id, int wmID)
{
	if (wmID > -1 && wmID < worldMatrix.size())
	{
		float xScale = textureRes[id].x / 1980;
		float yScale = textureRes[id].y / 1080;

		// setpos
		worldMatrix[wmID][0].w = center.x;
		worldMatrix[wmID][1].w = center.y;
		worldMatrix[wmID][2].w = 0.0f;
		// set scale
		worldMatrix[wmID][0].x = xScale;
		worldMatrix[wmID][1].y = yScale;

		textureInUse = textureIndexList[id];
	}
}

void StaticTextureBoxes::fromPosToQuadScreen(glm::vec2 positions, int id)
{
	if (id > -1 && id < worldMatrix.size())
	{
		// setpos
		worldMatrix[id][0].w = positions.x;
		worldMatrix[id][1].w = positions.y;
		worldMatrix[id][2].w = 0.0f;
	}
}

int StaticTextureBoxes::checkCollision(glm::vec2 mpos, float &newSoundProcent, float &xPos)
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

void StaticTextureBoxes::scaleBar(int id, float procentOfMax, bool fromRight) 
{
	pivot = glm::vec3(1.0f, 0.0f, 0.0f);

	float scale = (textureRes[0].x * procentOfMax) / 1920.0f;
	worldMatrix[0][0].x = scale;

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

std::string StaticTextureBoxes::getText()
{
	return "";
}
void StaticTextureBoxes::removeLastInput()
{
}
void StaticTextureBoxes::cleanText()
{

}

void StaticTextureBoxes::dontRender(bool dont)
{
	show = dont;
}