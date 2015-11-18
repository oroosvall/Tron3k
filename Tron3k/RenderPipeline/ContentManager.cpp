#include "ContentManager.h"
#include "SimpleShit.h"


void ContentManager::init()
{
	playerModels = new PlayerObject[1];

	TestMesh temp;

	temp.make();
	temp.take(playerModels[0].meshID, playerModels[0].vao, playerModels[0].index, playerModels[0].facecount);

	Mesh m;
	m.init(0, 0, 0);
	m.load("GameFiles/TestFiles/cube.v");

	Mesh m2;
	m2.init(0, 5, 0);
	m2.load("GameFiles/TestFiles/sphere.v");

	meshes.push_back(m);
	meshes.push_back(m2);

}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)meshes[i].getWorld());

		glBindVertexArray(meshes[i].vao);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].ibo);

		glDrawElements(GL_TRIANGLES, meshes[i].faceCount * 3, GL_UNSIGNED_SHORT, 0);
	}
	

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