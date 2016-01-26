#include "Text.h"

int Text::ScreenResWidth = 0;
int Text::ScreenResHeight = 0;

Text::Text(std::string _text, int _fontSize, GLuint textureId, vec2 _screenPos)
{
	text = _text;
	fontSize = _fontSize;
	texture = textureId;

	_screenPos.y = Text::ScreenResHeight - _screenPos.y;
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
	_screenPos.y = Text::ScreenResHeight - _screenPos.y;
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
	vec2 vertex_up_right;
	vec2 vertex_down_right;
	vec2 vertex_up_left;
	vec2 vertex_down_left;
	vec2 uv_up_left;
	vec2 uv_up_right;
	vec2 uv_down_right;
	vec2 uv_down_left;

	vector<Float5> verts;

	int nrOfLineBreaks = 0;
	int lineBreakPos = 0;

	
	for (unsigned int c = 0; c < text.length(); c++)
	{
		char character = text[c];
		if (character != '\n')
		{

			vec2 halfScreen = vec2(Text::ScreenResWidth / 2, Text::ScreenResHeight / 2);
			vertex_up_right = vec2(screenPos.x + (c - lineBreakPos)*fontSize + fontSize, screenPos.y + fontSize - (fontSize)*nrOfLineBreaks);
			vertex_up_right -= halfScreen;
			vertex_up_right /= halfScreen;
			vertex_down_right = vec2(screenPos.x + (c - lineBreakPos)*fontSize + fontSize, screenPos.y - (fontSize)*nrOfLineBreaks);
			//if (vertex_down_right.x > rightLimit)
			//	rightLimit = (int)vertex_down_right.x;
			//if (vertex_down_right.y < bottomLimit)
			//	bottomLimit = (int)vertex_down_right.y;
			vertex_down_right -= halfScreen;
			vertex_down_right /= halfScreen;
			vertex_up_left = vec2(screenPos.x + (c - lineBreakPos)*fontSize, screenPos.y + fontSize - (fontSize)*nrOfLineBreaks);
			vertex_up_left -= halfScreen;
			vertex_up_left /= halfScreen;
			vertex_down_left = vec2(screenPos.x + (c - lineBreakPos)*fontSize, screenPos.y - (fontSize)*nrOfLineBreaks);
			vertex_down_left -= halfScreen;
			vertex_down_left /= halfScreen;

			float uv_x = (character % 16) / 16.0f;
			float uv_y = (character / 16) / 16.0f;

			uv_up_left = vec2(uv_x, 1.0f - uv_y);
			uv_up_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
			uv_down_right = vec2(uv_x + 1.0f / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
			uv_down_left = vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));

			Float5 vert = { 0.0f };

			// top left
			vert.x = vertex_up_left.x;
			vert.y = vertex_up_left.y;
			vert.u = uv_up_left.x;
			vert.v = uv_up_left.y;

			verts.push_back(vert);

			// down left
			vert.x = vertex_down_left.x;
			vert.y = vertex_down_left.y;
			vert.u = uv_down_left.x;
			vert.v = uv_down_left.y;

			verts.push_back(vert);

			// down right
			vert.x = vertex_down_right.x;
			vert.y = vertex_down_right.y;
			vert.u = uv_down_right.x;
			vert.v = uv_down_right.y;

			verts.push_back(vert);

			// down left
			vert.x = vertex_up_right.x;
			vert.y = vertex_up_right.y;
			vert.u = uv_up_right.x;
			vert.v = uv_up_right.y;

			verts.push_back(vert);

		}
		else
		{
			lineBreakPos = c + 1;
			nrOfLineBreaks++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Float5)* verts.size(), &verts[0], GL_STATIC_DRAW);
	
	quadCount = verts.size();

}

void Text::draw()
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glDrawArrays(GL_QUADS, 0, quadCount);
}