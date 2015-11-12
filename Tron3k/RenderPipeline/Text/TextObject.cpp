#include "TextObject.h"

int TextObject::ScreenResWidth = 0;
int TextObject::ScreenResHeight = 0;

TextObject::TextObject(string str, int fontSize, vec2 screenPos)
{
	size = fontSize;

	screenPos.y = TextObject::ScreenResHeight - screenPos.y;
	pos = screenPos;
	rightLimit = 0; bottomLimit = TextObject::ScreenResHeight;

	setupText(str, pos);
}

TextObject::TextObject(TextObject* obj, vec2 screenPos)
{
	size = obj->size;
	if (screenPos.x != -1 || screenPos.y != -1)
	{
		screenPos.y = TextObject::ScreenResHeight - screenPos.y;
		pos = screenPos;
	}
	else
		pos = obj->pos;

	setupText(obj->text, pos);
}

void TextObject::setupText(string str, vec2 screenPos)
{
	text = str;

	vec2 vertex_up_right;
	vec2 vertex_down_right;
	vec2 vertex_up_left;
	vec2 vertex_down_left;
	vec2 uv_up_left;
	vec2 uv_up_right;
	vec2 uv_down_right;
	vec2 uv_down_left;
	int nrOfLineBreaks = 0;
	int lineBreakPos = 0;

	for (unsigned int c = 0; c < str.length(); c++)
	{
		char character = str[c];
		if (character != '\n')
		{
			vec2 halfScreen = vec2(TextObject::ScreenResWidth / 2, TextObject::ScreenResHeight / 2);
			vertex_up_right = vec2(screenPos.x + (c - lineBreakPos)*size + size, screenPos.y + size - (size)*nrOfLineBreaks);
			vertex_up_right -= halfScreen;
			vertex_up_right /= halfScreen;
			vertex_down_right = vec2(screenPos.x + (c - lineBreakPos)*size + size, screenPos.y - (size)*nrOfLineBreaks);
			if (vertex_down_right.x > rightLimit)
				rightLimit = (int)vertex_down_right.x;
			if (vertex_down_right.y < bottomLimit)
				bottomLimit = (int)vertex_down_right.y;
			vertex_down_right -= halfScreen;
			vertex_down_right /= halfScreen;
			vertex_up_left = vec2(screenPos.x + (c - lineBreakPos)*size, screenPos.y + size - (size)*nrOfLineBreaks);
			vertex_up_left -= halfScreen;
			vertex_up_left /= halfScreen;
			vertex_down_left = vec2(screenPos.x + (c - lineBreakPos)*size, screenPos.y - (size)*nrOfLineBreaks);
			vertex_down_left -= halfScreen;
			vertex_down_left /= halfScreen;

			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			uv_up_left = vec2(uv_x, 1.0f - uv_y);
			uv_up_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
			uv_down_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
			uv_down_left = vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));

			vertices.push_back(vertex_up_left);
			vertices.push_back(uv_up_left);

			vertices.push_back(vertex_down_left);
			vertices.push_back(uv_down_left);

			vertices.push_back(vertex_up_right);
			vertices.push_back(uv_up_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(uv_down_right);

			vertices.push_back(vertex_up_right);
			vertices.push_back(uv_up_right);

			vertices.push_back(vertex_down_left);
			vertices.push_back(uv_down_left);
		}
		else
		{
			lineBreakPos = c + 1;
			nrOfLineBreaks++;
		}
	}
}

TextObject::~TextObject()
{
	
}

void TextObject::getBuffer(int &fSize, vector<vec2> &dataBuf)
{
	fSize = size;
	dataBuf = vertices;
}

void TextObject::setText(string str)
{
	vertices.clear();
	setupText(str, pos);
}