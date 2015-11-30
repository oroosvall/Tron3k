#include "ContentManager.h"
#include "Texture.h"

void ContentManager::init()
{
	playerModels = new PlayerObject[1];

	TextureLookup tex;

	//0
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 0;
	tex.textureName = "GameFiles/TestFiles/Normal.png";
	textures.push_back(tex);
	//1
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/Diffuse.png";
	textures.push_back(tex);
	//2
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/sphere_n.png";
	textures.push_back(tex);
	//3
	//skybox
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/skybox.jpg";
	textures.push_back(tex);
	//4
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/Blank_normal.png";
	textures.push_back(tex);
	//5
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/GlowMap.png";
	textures.push_back(tex);
	//6
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/EmptyStaticGlowSpec.png";
	textures.push_back(tex);

	//7
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/player.png";
	textures.push_back(tex);

	//8
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/Blank_normal_50glow.png";
	textures.push_back(tex);
	//9
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/playerglow.png";
	textures.push_back(tex);
	//10
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/bullet_texture.png";
	textures.push_back(tex);
	//11
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/bulletGlow.png";
	textures.push_back(tex);


	Mesh m;
	m.init(0, -3, 0);
	m.load("GameFiles/TestFiles/cube.v");

	Mesh m2;
	m2.init(0, -8, 5);
	m2.load("GameFiles/TestFiles/sphere.v");

	//Skybox
	skybox.init(0, 0, 0);
	skybox.load("GameFiles/TestFiles/Skybox.v");

	meshes.push_back(m);
	meshes.push_back(m2);

	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i].textureID = loadTexture(textures[i].textureName);
		textures[i].loaded = true;
	}
	
	meshes[0].setTexture(textures[1].textureID);
	meshes[1].setTexture(textures[1].textureID);

	skybox.setTexture(textures[3].textureID);

	testMap.init();

	Mesh player;
	player.init(0, 0, 0);
	player.load("GhostBoss1.v");

	playerModels[0].meshID = player.vbo;
	playerModels[0].vao = player.vao;
	playerModels[0].index = player.ibo;
	playerModels[0].facecount = player.faceCount;

	bullet.init(0, 0, 0);
	bullet.load("GameFiles/TestFiles/bullet.v");

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
	testMap.release();

}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint portal_shader, GLuint portal_world)
{
	//portal depth write test
	testMap.portals[0].render();

	glUseProgram(portal_shader);
	glm::mat4 alreadyinworldpace;
	glProgramUniformMatrix4fv(portal_shader, portal_world, 1, GL_FALSE, &alreadyinworldpace[0][0]);

	GLuint test;
	glGenQueries(1, &test);
	glBeginQuery(GL_SAMPLES_PASSED, test);
	
	testMap.portals[0].render();





	glUseProgram(shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[1].textureID);
	glActiveTexture(GL_TEXTURE0 +1 );
	glBindTexture(GL_TEXTURE_2D, textures[0].textureID);
	//static glow
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, textures[5].textureID);
	testMap.render(shader, shaderLocation, textureLocation, normalLocation, glowSpecLocation);

	for (size_t i = 0; i < meshes.size(); i++)
	{
		glProgramUniformMatrix4fv(shader, shaderLocation, 1, GL_FALSE, (GLfloat*)meshes[i].getWorld());
	
		glProgramUniform1i(shader, textureLocation, 0);
		glProgramUniform1i(shader, normalLocation, 1);
		glProgramUniform1i(shader, glowSpecLocation, 2);
		//diffuse
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, meshes[i].textureID);
	
		 //normal dynamic glow
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, textures[0].textureID);
		
		//static glow
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[5].textureID);
		

		glBindVertexArray(meshes[i].vao);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].ibo);
	
		glDrawElements(GL_TRIANGLES, meshes[i].faceCount * 3, GL_UNSIGNED_SHORT, 0);
	}

	GLint passed = 2222;
	glEndQuery(GL_SAMPLES_PASSED);
	glGetQueryObjectiv(test, GL_QUERY_RESULT, &passed);

	if (passed == GL_FALSE)
		int k = 33;
	else if (passed > 0)
		int k = 33;
	
}

void ContentManager::renderPlayer(int playerID, glm::mat4 world)
{
	if (playerID == 0)
	{
		//diffuse
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[7].textureID);

		//normal dynamic glow
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[8].textureID);

		//static glow
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[9].textureID);

		glBindVertexArray(playerModels[playerID].vao);
		glBindBuffer(GL_ARRAY_BUFFER, playerModels[playerID].meshID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerModels[playerID].index);

		glDrawElements(GL_TRIANGLES, playerModels[playerID].facecount * 3, GL_UNSIGNED_SHORT, 0);
	}
	else if (playerID == 1)
	{
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[3].textureID);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[4].textureID);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[6].textureID);

		glBindVertexArray(skybox.vao);
		glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ibo);

		glDrawElements(GL_TRIANGLES, skybox.faceCount * 3, GL_UNSIGNED_SHORT, 0);

		glEnable(GL_DEPTH_TEST);
	}
	else if (playerID == 2)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[10].textureID);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[4].textureID);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[11].textureID);

		glBindVertexArray(bullet.vao);
		glBindBuffer(GL_ARRAY_BUFFER, bullet.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bullet.ibo);

		glDrawElements(GL_TRIANGLES, bullet.faceCount * 3, GL_UNSIGNED_SHORT, 0);
	}
}

void ContentManager::renderPortals(GLuint shader, GLuint world)
{
	glUseProgram(shader);
	glm::mat4 alreadyinworldpace;
	glProgramUniformMatrix4fv(shader, world, 1, GL_FALSE, &alreadyinworldpace[0][0]);
	testMap.renderPortals();
}

std::vector<std::vector<float>> ContentManager::getMeshBoxes()
{
	std::vector<std::vector<float>> size;

	for (size_t i = 0; i < meshes.size(); i++)
	{
		size.push_back(meshes[i].getBoundaries());
	}

	//Size is currently a vector containing the extremes for all of our world objects
	return size;
}