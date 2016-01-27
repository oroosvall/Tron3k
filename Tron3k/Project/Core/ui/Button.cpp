#include "Button.h"

Button::Button()
{
	textureIndexList[0] = -1;
	textureIndexList[1] = -1;
	textureIndexInUse = -1;
	worldMatrix[0] = { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
					};
	worldMatrix[1] = { 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	uniqueKey = -1;
	nrOfButtons = 0;
}
Button::Button(glm::vec2 positions[], glm::vec2 uv[], int textureId1, int textureId2, int uniqueKey, GLuint shader, GLuint worldMat, GLuint textureLocation) //Kolla mer på denna
{
	glm::vec2 posXY[4];
	glm::vec2 uv2[4];
	for (int i = 0; i < 4; i++)
	{
		posXY[i] = positions[i];
		uv2[i] = uv[i];
	}

	pos[0] = uiVertex(posXY, uv2);

	textureIndexList[0] = textureId1;
	textureIndexList[1] = textureId2;
	this->uniqueKey = uniqueKey;
	textureIndexInUse = textureId1;
	nrOfButtons = 1;

	this->uniformTextureLocation = uniformTextureLocation;
	this->uniformWorldMatrix = uniformWorldMatrix;
	this->shader = shader;
}
Button::~Button() {}

void Button::render(std::vector<GLuint> textureIds, int i, GLuint gVertexAttribute, GLuint gVertexBuffer)
{
	//Kolla i den andra koden ifall textur aktiveringen och bindningen ska se ut så där.;
	//Activate the buttons texture

	glActiveTexture(GL_TEXTURE0 + textureIds[textureIndexInUse]);
	glBindTexture(GL_TEXTURE_2D, textureIds[textureIndexInUse]);

	glProgramUniform1i(shader, uniformTextureLocation, i);
	glProgramUniformMatrix4fv(shader, uniformWorldMatrix, 1, GL_FALSE, &worldMatrix[0][0][0]);

	//Bind the vertex buffer that will be used
	glBindVertexArray(gVertexAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Button::setWorldMatrix(float x, float y, int id)
{
	worldMatrix[id][0].w = x;
	worldMatrix[id][1].w = y;
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
	worldMatrix[id][0].x = scale;
	worldMatrix[id][1].y = scale;
	worldMatrix[id][2].z = scale;
}

int Button::checkCollision(glm::vec2 mpos)
{
	int returnValue = -1;
	for (int i = 0; i < nrOfButtons; i++)
	{
		if (mpos.x >= pos[i].vertexList[1].x && mpos.x <= pos[i].vertexList[2].x)
		{
			if (mpos.y >= pos[i].vertexList[1].y && mpos.y <= pos[i].vertexList[2].y)
			{
				returnValue = uniqueKey;
			}
		}
	}
	
	return returnValue;
}