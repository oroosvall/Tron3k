#include "HideAble.h"

HideAble::HideAble()
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
	nrOfTextures = 0;

	winX = 0;
	winY = 0;
	startWMX = 0;
	startWMY = 0;
}

HideAble::HideAble(glm::vec2 center, int* textureId1, int nrOfTextures, IRenderPipeline* uiRender, std::vector<glm::vec2> textRes, glm::vec3 offSetsTextSize)
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

	this->winX = winX;
	this->winY = winY;

	float xScale = textureRes[0].x / 1980;
	float yScale = textureRes[0].y / 1080;

	// setpos
	worldMatrix[0][0].w = center.x;
	worldMatrix[0][1].w = center.y;
	worldMatrix[0][2].w = 0.0f;
	// set scale
	worldMatrix[0][0].x = xScale;
	worldMatrix[0][1].y = yScale;

	originalWM = worldMatrix[0];
	
	glm::vec2 pos[2];

	pos[0] = glm::vec2(worldMatrix[0][0].w - worldMatrix[0][0].x, worldMatrix[0][1].w - worldMatrix[0][1].y);
	pos[1] = glm::vec2(worldMatrix[0][0].w + worldMatrix[0][0].x, worldMatrix[0][1].w + worldMatrix[0][1].y);

	//if (gotText == 1)
	//{
	//	yOffSet = 0;
	//	xOffSet = 0;
	//	//Put offsets and text size in a vec3 since since my construct is abit long
	//	if (offSetsTextSize.y != 0)
	//		yOffSet = -((offSetsTextSize.y - 1) * 0.5) * winY;
	//	if (offSetsTextSize.x != 0)
	//		xOffSet = glm::vec2((offSetsTextSize.x + 1) * 0.5).x * winX;
	//
	//	text = uiRender->createTextObject("", offSetsTextSize.z, (glm::vec2(((pos[0].x + 1) * 0.5) * winX + xOffSet, -((pos[0].y - 1)* 0.5) * winY - yOffSet)));
	//}
	//this->gotText = gotText;
}

HideAble::~HideAble()
{
	if (textureRes != nullptr)
		delete[] textureRes;
	if (textureIndexList != nullptr)
		delete[] textureIndexList;
	worldMatrix.clear();
}

void HideAble::renderText(int id)
{
}

void HideAble::renderQuad(int id)
{
	//uiRender->ui_renderQuad((float*)&worldMatrix[0][0], (float*)&pivot.x, textureInUse, 1.0f, id);
	for (int i = 0; i < worldMatrix.size(); i++)
		uiRender->ui_renderQuad((float*)&worldMatrix[i][0], (float*)&pivot.x, textureInUse, 1.0f, id);
}

void HideAble::setWorldMatrix(int id, float x, float y)
{
	if (id > -1 && id < worldMatrix.size())
	{
		worldMatrix[id][0].w += x;
		worldMatrix[id][1].w += y;
	}
}

void HideAble::resetWorldMatrix(int id)
{
	if (id > -1 && id < worldMatrix.size())
	{
		worldMatrix[id][0].w = startWMX;
		worldMatrix[id][1].w = startWMY;
	}
}


//We create a new WM since we render the same object multiple times but with different WM so it looks like different objects.
//Sub and add numbers poping up on screen for score and tokens uses this.
int HideAble::addNewWM()
{
	//Create a new WM
	worldMatrix.push_back(originalWM);
	//Return the new WMs index
	return worldMatrix.size() - 1;
}

//We need to delete them since they only exists for a few seconds
void HideAble::deleteOldestWM()
{
	//Delete the oldest wm since it goes away first
	int tmpSize = worldMatrix.size();
	if (tmpSize > 0)
		worldMatrix.erase(worldMatrix.begin());
}

void HideAble::changeTexUsed(int id, int wmID)
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

void HideAble::setTexture(std::vector<GLuint> uiTextureIds)
{
	for (int i = 0; i < nrOfTextures; i++)
	{
		textureIndexList[i] = uiTextureIds[textureIndexList[i]];
	}
	textureInUse = textureIndexList[0];
}

/*
void HideAble::setText(std::string text)
{
	menuInputText += text;
	outPutLength += text.size();
	ingameText = text;
	uiRender->setTextObjectText(this->text, menuInputText); //Den ska använda ingameText när det gäller ingame saker.
}
std::string HideAble::getText()
{
	return menuInputText;
}
void HideAble::removeLastInput()
{
	if (outPutLength > 0)
	{
		menuInputText.pop_back();
		outPutLength--;
		uiRender->setTextObjectText(text, menuInputText);
	}
}
void HideAble::cleanText()
{
	menuInputText.clear();
	ingameText.clear();
	outPutLength = 0;

	uiRender->setTextObjectText(this->text, menuInputText);
}
*/