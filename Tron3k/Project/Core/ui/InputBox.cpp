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

	pivot = glm::vec3(0.0f);

	winX = 0;
	winY = 0;
	yOffSet = 0;
	xOffSet = 0;
}
InputBox::InputBox(glm::vec2 center, int textureId1, int uniqueKey, IRenderPipeline* uiRender, glm::vec2 textRes, int winX, int winY, glm::vec3 offSetsTextSize)
{
	this->uiRender = uiRender;
	this->center = center;
	this->textureRes = textRes;

	textureId = textureId1;
	this->uniqueKey = uniqueKey;
	hoverCheckKey = 0;

	this->winX = winX;
	this->winY = winY;

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

	yOffSet = 0;
	xOffSet = 0;
	//Put offsets and text size in a vec3 since since my construct is abit long
	if(offSetsTextSize.y != 0)
		yOffSet = -((offSetsTextSize.y - 1) * 0.5) * winY;
	if (offSetsTextSize.x != 0)
		xOffSet = glm::vec2((offSetsTextSize.x + 1) * 0.5).x * winX;

	//yOffSet = offSetsTextSize.y; //30
	//xOffSet = offSetsTextSize.x; //25

	text = uiRender->createTextObject("", offSetsTextSize.z, (glm::vec2(((pos[0].x + 1) * 0.5) * winX + xOffSet, -((pos[0].y - 1)* 0.5) * winY - yOffSet)));

	//double tX = (x / (double)winX) * 2 - 1.0; // (x/ResolutionX) * 2 - 1
	//double tY = (-y / (double)winY) * 2 + 1.0; // (y/ResolutionY) * 2 - 1


}
InputBox::~InputBox()
{
	uiRender->removeTextObject(text);
}

void InputBox::renderText(int id)
{
	uiRender->renderTextObject(text);
}

void InputBox::renderQuad(int id)
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
	textureId = uiTextureIds[textureId];
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

void InputBox::setWindowResolution(int winX, int winY)
{
	this->winX = winX;
	this->winY = winY;
}

void InputBox::setText(std::string text)
{
	menuInputText += text;
	outPutLength += text.size();
	ingameText = text; 
	uiRender->setTextObjectText(this->text, menuInputText); //Den ska använda ingameText när det gäller ingame saker.
}
std::string InputBox::getText()
{
	return menuInputText;
}
void InputBox::removeLastInput()
{
	if (outPutLength > 0)
	{
		menuInputText.pop_back();
		outPutLength--;
		uiRender->setTextObjectText(text, menuInputText);
	}
}
void InputBox::cleanText()
{
	menuInputText.clear();
	ingameText.clear();
	outPutLength = 0;

	uiRender->setTextObjectText(this->text, menuInputText);
}

