#include "Text.h"

int Text::ScreenResWidth = 0;
int Text::ScreenResHeight = 0;

int Text::ScreenResHeightBase = 720;
int Text::ScreenResWidthBase = 1080;

Text::Text(std::string _text, int _fontSize, GLuint textureId, vec2 _screenPos, bool _scale)
{
	text = _text;
	fontSize = _fontSize;
	texture = textureId;

	float yScale = (float)ScreenResHeight / (float)ScreenResHeightBase;
	float xScale = (float)ScreenResWidth / (float)ScreenResWidthBase;

	TextScale = _scale;

	if (TextScale)
	{
		_screenPos.y = Text::ScreenResHeight - (_screenPos.y * yScale);
		_screenPos.x *= yScale;
	}
	else
	{
		_screenPos.y = Text::ScreenResHeight - _screenPos.y;
	}
	screenPos = _screenPos;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Float5), BUFFER_OFFSET(0));
	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Float5), BUFFER_OFFSET(sizeof(float) * 3));

	fillBuffer();

}

Text::~Text()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Text::setPos(vec2 _screenPos)
{
	float yScale = (float)ScreenResHeight / (float)ScreenResHeightBase;
	float Scale = (float)ScreenResWidth / (float)ScreenResWidthBase;

	if (TextScale)
	{
		_screenPos.y = Text::ScreenResHeight - (_screenPos.y * yScale);
		_screenPos.x *= yScale;
	}
	else
	{
		_screenPos.y = Text::ScreenResHeight - _screenPos.y;
	}
	screenPos = _screenPos;
	fillBuffer();
}

void Text::setTexture(GLuint textureId)
{
	texture = textureId;
	fillBuffer();
}

void Text::setSize(int _fontSize)
{
	fontSize = _fontSize;
	fillBuffer();
}

void Text::setText(std::string _text)
{
	text = _text;
	fillBuffer();
}

void Text::fillBuffer()
{
	float xScale = 1.0f;
	if(TextScale)
		xScale = (float)ScreenResWidth / (float)ScreenResWidthBase;

	vec2 vertex_up_right;
	vec2 vertex_down_right;
	vec2 vertex_up_left;
	vec2 vertex_down_left;
	vec2 uv_up_left;
	vec2 uv_up_right;
	vec2 uv_down_right;
	vec2 uv_down_left;

	int index = 0;
	Float5* verts = new Float5[text.length() * 4];

	int nrOfLineBreaks = 0;
	int lineBreakPos = 0;

	int size = fontSize * xScale;

	float scale = 1.0f;
	
	float offsetX = 0;
	float offsetY = 0;

	for (unsigned int c = 0; c < text.length(); c++)
	{
		char character = text[c];

		if (character != '\n')
		{
			switch (character)
			{
			case '.':
				scale = 1.4f;
				break;	
			case ',':	
				scale = 1.4f;
				break;	
			case '-':	
				scale = 1.4f;
				break;	
			case '+':	
				scale = 1.4f;
				break;	
			case '*':	
				scale = 1.4f;
				break;	
			case '"':	
				scale = 1.4f;
				break;	
			case '|':	
				scale = 1.4f;
				break;	
			case ':':	
				scale = 1.8f;
				break;	
			case ';':	
				scale = 1.4f;
				break;	
			case '`':	
				scale = 1.4f;
				break;	
			case '´':	
				scale = 1.4f;
				break;	
			case 'l':	
				scale = 1.4f;
				break;	
			case 'I':	
				scale = 1.4f;
				break;	
			case 'i':
				scale = 1.4f;
				break;
			case '!':
				scale = 1.4f;
				break;
			case ' ':
				scale = 1.8f;
				break;
			default:
				scale = 1.0f;
				break;
			}

			size = fontSize / scale;

			vec2 halfScreen = vec2(Text::ScreenResWidth / 2, Text::ScreenResHeight / 2);

			vertex_up_right = vec2(screenPos.x + offsetX + size, screenPos.y + fontSize - (fontSize)*nrOfLineBreaks - offsetY);
			vertex_up_right -= halfScreen;
			vertex_up_right /= halfScreen;
			vertex_down_right = vec2(screenPos.x + offsetX + size, screenPos.y - (fontSize)*nrOfLineBreaks - offsetY);
			//if (vertex_down_right.x > rightLimit)
			//	rightLimit = (int)vertex_down_right.x;
			//if (vertex_down_right.y < bottomLimit)
			//	bottomLimit = (int)vertex_down_right.y;
			vertex_down_right -= halfScreen;
			vertex_down_right /= halfScreen;
			vertex_up_left = vec2(screenPos.x + offsetX, screenPos.y + fontSize - (fontSize)*nrOfLineBreaks - offsetY);
			vertex_up_left -= halfScreen;
			vertex_up_left /= halfScreen;
			vertex_down_left = vec2(screenPos.x + offsetX, screenPos.y - (fontSize)*nrOfLineBreaks - offsetY);
			vertex_down_left -= halfScreen;
			vertex_down_left /= halfScreen;

			offsetX += size;
			
			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			uv_up_left = vec2(uv_x, 1.0f - uv_y);
			uv_up_right = vec2(uv_x + 1.0f / 16.0f / scale, 1.0f - uv_y);
			uv_down_right = vec2(uv_x + 1.0f / 16.0f / scale, 1.0f - (uv_y + 1.0f / 16.0f));
			uv_down_left = vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));

			Float5 vert = { 0.0f };

			// top left
			vert.x = vertex_up_left.x;
			vert.y = vertex_up_left.y;
			vert.u = uv_up_left.x;
			vert.v = uv_up_left.y;

			verts[index] = vert;

			// down left
			vert.x = vertex_down_left.x;
			vert.y = vertex_down_left.y;
			vert.u = uv_down_left.x;
			vert.v = uv_down_left.y;

			verts[index + 1] = vert;

			// down right
			vert.x = vertex_down_right.x;
			vert.y = vertex_down_right.y;
			vert.u = uv_down_right.x;
			vert.v = uv_down_right.y;

			verts[index + 2] = vert;

			// down left
			vert.x = vertex_up_right.x;
			vert.y = vertex_up_right.y;
			vert.u = uv_up_right.x;
			vert.v = uv_up_right.y;

			verts[index + 3] = vert;

			index += 4;

		}
		else
		{
			lineBreakPos = c + 1;
			nrOfLineBreaks++;
			offsetX = 0;
			offsetY += 4;
		}

		centerOffset.x = (verts[index-1].x - verts[0].x) / 2;
		centerOffset.y = (verts[index-1].y - verts[0].y) / 2;

	}

	if (index != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Float5) * index, verts, GL_STATIC_DRAW);
	}
	quadCount = index;

	delete[] verts;

}

void Text::draw()
{

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glDrawArrays(GL_QUADS, 0, quadCount);
}