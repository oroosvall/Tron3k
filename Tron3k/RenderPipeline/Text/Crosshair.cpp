#include "Crosshair.h"

void Crosshair::init()
{

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float xPos = 0.02f;
	float xSize = Text::ScreenResWidth * xPos;
	float yPos = xSize / Text::ScreenResHeight;

	Float5 verts[4]
	{
		xPos ,yPos ,0 ,
		1 ,1 ,

		-xPos ,yPos ,0 ,
		0 ,1 ,

		-xPos ,-yPos ,0 ,
		0 ,0 ,

		xPos ,-yPos ,0 ,
		0 ,1 ,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Float5), BUFFER_OFFSET(0));
	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Float5), BUFFER_OFFSET(sizeof(float) * 3));

}

void Crosshair::release()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Crosshair::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	glDrawArrays(GL_QUADS, 0, 4);
}
