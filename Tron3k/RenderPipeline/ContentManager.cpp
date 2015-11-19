#include "ContentManager.h"
#include "SimpleShit.h"
#include "Texture.h"

void ContentManager::init()
{
	playerModels = new PlayerObject[1];

	TestMesh temp;

	temp.make();
	temp.take(playerModels[0].meshID, playerModels[0].vao, playerModels[0].index, playerModels[0].facecount);

	TextureLookup tex;

	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 0;
	tex.textureName = "GameFiles/TestFiles/Normal.png";
	textures.push_back(tex);

	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/Diffuse.png";
	textures.push_back(tex);

	Mesh m;
	m.init(0, 0, 0);
	m.load("GameFiles/TestFiles/cube.v");

	Mesh m2;
	m2.init(0, 5, 0);
	m2.load("GameFiles/TestFiles/sphere.v");

	meshes.push_back(m);
	meshes.push_back(m2);

	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i].textureID = loadTexture(textures[i].textureName);
		textures[i].loaded = true;
	}

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].setTexture(textures[i].textureID);
	}

}

ContentManager::~ContentManager()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glDeleteBuffers(1, &meshes[i].ibo);
		glDeleteBuffers(1, &meshes[i].vbo);
		glDeleteVertexArrays(1,&meshes[i].vao);
	}

	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].textureID != 0)
		{
			glDeleteTextures(1, &textures[i].textureID);
		}
	}

	glDeleteBuffers(1, &playerModels[0].meshID);
	glDeleteBuffers(1, &playerModels[0].index);
	glDeleteVertexArrays(1, &playerModels[0].vao);
	
	delete playerModels;

}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)meshes[i].getWorld());

		glProgramUniform1i(shader, textureLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshes[i].textureID);

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