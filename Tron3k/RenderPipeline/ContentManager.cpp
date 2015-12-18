#include "ContentManager.h"
#include "Texture.h"

void ContentManager::init()
{
	//init flags
	f_portal_culling = false;
	f_freeze_portals = false;
	f_render_chunks = true;
	f_render_abb = false;
	f_render_obb = false;

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

	//12
	tex.loaded = false;
	tex.textureID = 0;
	tex.fileTexID = 1;
	tex.textureName = "GameFiles/TestFiles/lightwall.png";
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
	nrChunks = testMap.chunks.size();
	renderedChunks = new bool[nrChunks];
	renderNextChunks = new bool[nrChunks];
	for (int n = 0; n < nrChunks; n++)
	{
		renderedChunks[n] = false;
		renderNextChunks[n] = false;
	}
	glGenQueries(1, &portalQuery);

	Mesh player;
	player.init(0, 0, 0);
	player.load("GhostBoss1.v");
	
	playerModels[0].meshID = player.vbo;
	playerModels[0].vao = player.vao;
	playerModels[0].index = player.ibo;
	playerModels[0].facecount = player.faceCount;
	
	bullet.init(0, 0, 0);
	bullet.load("GameFiles/TestFiles/bullet.v");

	//Anim files
	trapper_first_reload.init();
	trapper_first_reload.load("GameFiles/CharacterFiles/trapper_first_reload.bin");

	trapper_third_idle.init();
	trapper_third_idle.load("GameFiles/CharacterFiles/trapper_third_idle.bin");

	trapper_third_run.init();
	trapper_third_run.load("GameFiles/CharacterFiles/trapper_third_run.bin");

	trapper_third_jump_air.init();
	trapper_third_jump_air.load("GameFiles/CharacterFiles/trapper_third_jump_air.bin");

	trapper_third_jump_begin.init();
	trapper_third_jump_begin.load("GameFiles/CharacterFiles/trapper_third_jump_begin.bin");

	trapper_third_jump_end.init();
	trapper_third_jump_end.load("GameFiles/CharacterFiles/trapper_third_jump_end.bin");

	trapper_third_strafe_left.init();
	trapper_third_strafe_left.load("GameFiles/CharacterFiles/trapper_third_strafe_left.bin");

	trapper_third_strafe_right.init();
	trapper_third_strafe_right.load("GameFiles/CharacterFiles/trapper_third_strafe_right.bin");
}

void ContentManager::release()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glDeleteBuffers(1, &meshes[i].ibo);
		glDeleteBuffers(1, &meshes[i].vbo);
		glDeleteVertexArrays(1, &meshes[i].vao);
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

	glDeleteQueries(1, &portalQuery);

	delete[] renderedChunks;
	delete[] renderNextChunks;

	trapper_first_reload.release();
	trapper_third_idle.release();
	trapper_third_run.release();
	trapper_third_jump_air.release();
	trapper_third_jump_begin.release();
	trapper_third_jump_end.release();
	trapper_third_strafe_left.release();
	trapper_third_strafe_right.release();

	skybox.release();
	bullet.release();

	//collision render free
	for (int c = 0; c < nrChunks; c++)
	{
		int nrABB = testMap.chunks[c].collisionRender.abbRender.size();

		for (int a = 0; a < nrABB; a++)
		{
			testMap.chunks[c].collisionRender.abbRender[a].abbBoxR.release();
			int nrObb = testMap.chunks[c].collisionRender.abbRender[a].obbBoxesR.size();

			for (int b = 0; b < nrObb; b++)
				testMap.chunks[c].collisionRender.abbRender[a].obbBoxesR[b].release();
		}
	}
}

ContentManager::~ContentManager()
{
	
}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint portal_shader, GLuint portal_world)
{
	glUseProgram(shader);
	//diffuse
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[1].textureID);
	//normal & dynamic glow

	glActiveTexture(GL_TEXTURE0 +1 );
	glBindTexture(GL_TEXTURE_2D, textures[8].textureID);
	//static glow & spec
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, textures[6].textureID);

	glProgramUniform1i(shader, textureLocation, 0);
	glProgramUniform1i(shader, normalLocation, 1);
	glProgramUniform1i(shader, glowSpecLocation, 2);

	//reset rendered chunks from last frame
	for (int n = 0; n < nrChunks; n++)
		renderedChunks[n] = false;

	//render chunks logged from last frame
	for (int n = 0; n < nrChunks; n++)
	{
		if (renderNextChunks[n] || f_portal_culling == false)
		{
			//Glow values for world
			glProgramUniform3fv(shader, DglowColor, 1, (GLfloat*)&testMap.chunks[n].color[0]);
			glProgramUniform1f(shader, SglowColor, testMap.chunks[n].staticIntes);
			if (f_render_chunks)
				testMap.renderChunk(shader, shaderLocation, n);
			renderedChunks[n] = true;
		}
	}

	//reset the renderrednextlist
	if(f_freeze_portals == false)
		for (int n = 0; n < nrChunks; n++)
			renderNextChunks[n] = false;

	glUseProgram(portal_shader);
	glm::mat4 alreadyinworldpace;
	glProgramUniformMatrix4fv(portal_shader, portal_world, 1, GL_FALSE, &alreadyinworldpace[0][0]);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	if (f_freeze_portals == false)
	{
		//render portals from the rendered chunks
		for (int n = 0; n < nrChunks; n++)
		{
			if (renderedChunks[n] == true)
			{
				int size = testMap.chunks[n].portals.size();
				for (int p = 0; p < size; p++) // render the portals
				{
					// dont render if it bridges between chunks that are already in the rendernextqueue
					if (renderNextChunks[testMap.chunks[n].portals[p].bridgedRooms[0]] == false ||
						renderNextChunks[testMap.chunks[n].portals[p].bridgedRooms[1]] == false)
					{
						glBeginQuery(GL_SAMPLES_PASSED, portalQuery);
						testMap.chunks[n].portals[p].render();
						GLint passed = 2222;
						glEndQuery(GL_SAMPLES_PASSED);
						glGetQueryObjectiv(portalQuery, GL_QUERY_RESULT, &passed);

						if (passed > 0)
						{
							renderNextChunks[testMap.chunks[n].portals[p].bridgedRooms[0]] = true;
							renderNextChunks[testMap.chunks[n].portals[p].bridgedRooms[1]] = true;
						}
					}
				}
			}
		}
		renderNextChunks[0] = true;
		renderNextChunks[testMap.currentChunk] = true;
	}

	if (f_render_abb || f_render_obb)
	{
		//render collision boxes
		for (int c = 0; c < nrChunks; c++)
		{

			ChunkCollision* col = testMap.chunks[c].getChunkCollision();

			float nrABB = col->abbStuff.size();
			for (int n = 0; n < nrABB; n++)
			{

				if (f_render_abb)
				{
					testMap.chunks[c].collisionRender.abbRender[n].abbBoxR.BindVertData();
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
				}

				int nrObb = testMap.chunks[c].collisionRender.abbRender[n].obbBoxesR.size();
				for (int k = 0; k < nrObb; k++)
				{
					if (f_render_obb)
					{
						testMap.chunks[c].collisionRender.abbRender[n].obbBoxesR[k].BindVertData();
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
					}
				}
			}
		}
	}
}

void ContentManager::renderPlayer(int renderID, int keyframe, glm::mat4 world, GLuint uniformKeyMatrixLocation)
{
	if (renderID == -3) //skybox
	{
		glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

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
		//glDisable(GL_BLEND);
	}
	else if (renderID == -2) //bullet
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[10].textureID);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[8].textureID);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[11].textureID);

		glBindVertexArray(bullet.vao);
		glBindBuffer(GL_ARRAY_BUFFER, bullet.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bullet.ibo);

		glDrawElements(GL_TRIANGLES, bullet.faceCount * 3, GL_UNSIGNED_SHORT, 0);
	}

	// ----------- Character Animations ---------- //
	
	if (renderID > -1)
	{
		//pick texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[7].textureID);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textures[8].textureID);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textures[9].textureID);

		switch (AnimationState(renderID))
		{
		case first_idle:		trapper_first_reload.update(0);				trapper_first_reload.draw(uniformKeyMatrixLocation);	break;
		case first_run:			trapper_first_reload.update(0);				trapper_first_reload.draw(uniformKeyMatrixLocation);	break;
		case first_air:			trapper_first_reload.update(0);				trapper_first_reload.draw(uniformKeyMatrixLocation);	break;

		case first_fire:		trapper_first_reload.update(0);				trapper_first_reload.draw(uniformKeyMatrixLocation);	break;
		case first_reload:		trapper_first_reload.update(keyframe);		trapper_first_reload.draw(uniformKeyMatrixLocation);	break;

		case third_idle:		trapper_third_idle.update(keyframe);		trapper_third_idle.draw(uniformKeyMatrixLocation);		break;

		case third_run:			trapper_third_run.update(keyframe);			trapper_third_run.draw(uniformKeyMatrixLocation);       break;
		case third_run_rev:		trapper_third_run.update(keyframe);			trapper_third_run.draw(uniformKeyMatrixLocation);		break;

		case third_strafe_left:		trapper_third_strafe_left.update(keyframe);			trapper_third_strafe_left.draw(uniformKeyMatrixLocation);		break;
		case third_strafe_right:	trapper_third_strafe_right.update(keyframe);			trapper_third_strafe_right.draw(uniformKeyMatrixLocation);		break;


		case third_air:			trapper_third_jump_air.update(keyframe);	trapper_third_jump_air.draw(uniformKeyMatrixLocation);		break;

		case third_jump_begin:	trapper_third_jump_begin.update(keyframe);	trapper_third_jump_begin.draw(uniformKeyMatrixLocation);		break;
		case third_jump_end:	trapper_third_jump_end.update(keyframe);	trapper_third_jump_end.draw(uniformKeyMatrixLocation);		break;

		case third_death:		trapper_third_idle.update(keyframe);		trapper_third_idle.draw(uniformKeyMatrixLocation);		break;

		default:				break;
		}
	}
}

void* ContentManager::getChunkCollisionVectorAsPointer(int chunkID)
{
	if (chunkID < nrChunks)
		return (void*)testMap.getChunkCollision(chunkID);
	return nullptr;
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

void ContentManager::bindLightwalTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[12].textureID);
}