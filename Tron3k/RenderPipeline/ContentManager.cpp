#include "ContentManager.h"
#include "SimpleShit.h"


void ContentManager::init()
{
	playerModels = new PlayerObject[1];

	TestMesh temp;

	temp.make();
	temp.take(playerModels[0].meshID, playerModels[0].vao, playerModels[0].index, playerModels[0].facecount);

	tempMesh.init(0, 0, 0);
	tempMesh.load("GameFiles/TestFiles/cube.v");

}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation)
{

	glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)tempMesh.getWorld());

	glBindVertexArray(tempMesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, tempMesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMesh.ibo);

	glDrawElements(GL_TRIANGLES, tempMesh.faceCount * 3, GL_UNSIGNED_SHORT, 0);

}

void ContentManager::renderPlayer(int playerID, glm::mat4 world)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, playerModels[playerID].textureID);
	glBindVertexArray(playerModels[playerID].vao);
	glBindBuffer(GL_ARRAY_BUFFER, playerModels[playerID].meshID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerModels[playerID].index);

	glDrawElements(GL_TRIANGLES, playerModels[playerID].facecount * 3, GL_UNSIGNED_SHORT, 0);
}