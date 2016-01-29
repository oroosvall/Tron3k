#include "ContentManager.h"
#include "..\Texture.h"

#include "../Utils/TimeQuery.h"

void ContentManager::init()
{
	//init flags
	f_portal_culling = false;
	f_freeze_portals = false;
	f_render_chunks = true;
	f_render_abb = false;
	f_render_obb = false;

	blank_diffuse = loadTexture("GameFiles/Textures/blank_d.dds");
	blank_normal = loadTexture("GameFiles/Textures/blank_n.dds");
	blank_glow = loadTexture("GameFiles/Textures/blank_g.dds");

	skyTexture = loadTexture("GameFiles/Textures/skybox.dds");

	playerCharacters[0].load("trapper");

	playerCharacters[1].load("destroyer");

	playerCharacters[2].load("shanker");

	playerCharacters[3].load("brute");

	playerCharacters[4].load("manipulator");


	int* lengths = playerCharacters[0].loadAnimations("trapper");
	for (int i = 0; i < AnimationState::none; i++)
		keyFrameLengths[0 * AnimationState::none + i] = lengths[i];

	lengths = playerCharacters[1].loadAnimations("destroyer");
	for (int i = 0; i < AnimationState::none; i++)
		keyFrameLengths[1 * AnimationState::none + i] = lengths[i];

	lengths = playerCharacters[2].loadAnimations("shanker");
	for (int i = 0; i < AnimationState::none; i++)
		keyFrameLengths[2 * AnimationState::none + i] = lengths[i];

	lengths = playerCharacters[3].loadAnimations("brute");
	for (int i = 0; i < AnimationState::none; i++)
		keyFrameLengths[3 * AnimationState::none + i] = lengths[i];

	lengths = playerCharacters[4].loadAnimations("manipulator");
	for (int i = 0; i < AnimationState::none; i++)
		keyFrameLengths[4 * AnimationState::none + i] = lengths[i];

	trapperBullet.load("GameFiles/CharacterFiles/primary_trapper.bin");
	trapperConsume.load("GameFiles/CharacterFiles/consumable_Trapper.bin");
	shankerBullet.load("GameFiles/CharacterFiles/primary_shanker.bin");
	shankerSpecial.load("GameFiles/CharacterFiles/special_shanker.bin");
	bruteThunderDome.load("GameFiles/CharacterFiles/special_Punisher.bin");

	//Skybox
	skybox.init(0, 0, 0);
	skybox.load("GameFiles/TestFiles/Skybox.v");

	testMap.init();
	nrChunks = testMap.chunks.size();
	if (nrChunks > 0)
	{
		if(nrChunks == 1)
			f_portal_culling = false;

		renderedChunks = new bool[nrChunks];
		renderNextChunks = new bool[nrChunks];
		for (int n = 0; n < nrChunks; n++)
		{
			renderedChunks[n] = false;
			renderNextChunks[n] = false;
		}
	}
	else
		f_portal_culling = false;

	glGenQueries(1, &portalQuery);
	
}

void ContentManager::release()
{
	glDeleteTextures(1, &blank_diffuse);
	glDeleteTextures(1, &blank_normal);
	glDeleteTextures(1, &blank_glow);

	glDeleteTextures(1, &skyTexture);

	testMap.release();

	glDeleteQueries(1, &portalQuery);

	if (nrChunks > 0)
	{
		delete[] renderedChunks;
		delete[] renderNextChunks;
	}

	skybox.release();

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

	for (int i = 0; i < 5; i++)
	{
		playerCharacters[i].release();
	}

	trapperBullet.release();
	trapperConsume.release();
	shankerBullet.release();
	shankerSpecial.release();

	bruteThunderDome.release();

}

ContentManager::~ContentManager()
{
	
}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint portal_shader, GLuint portal_world)
{
	glUseProgram(shader);
	//diffuse
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[1].textureID);
	//normal & dynamic glow

	//glActiveTexture(GL_TEXTURE0 +1 );
	//glBindTexture(GL_TEXTURE_2D, textures[8].textureID);
	////static glow & spec
	//glActiveTexture(GL_TEXTURE0 + 2);
	//glBindTexture(GL_TEXTURE_2D, textures[6].textureID);

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

	int portal = startTimer("Portals");

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
		//set enviroment chunk to true
		renderNextChunks[0] = true;
		renderNextChunks[testMap.currentChunk] = true;
	}

	stopTimer(portal);

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

		testMap.renderCapAbb();

	}
}

void ContentManager::renderMisc(int renderID)
{
	if (renderID == -3) //skybox
	{
		glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTexture);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, blank_normal);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, blank_glow);

		glBindVertexArray(skybox.vao);
		glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ibo);

		glDrawElements(GL_TRIANGLES, skybox.faceCount * 3, GL_UNSIGNED_SHORT, 0);

		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);
	}
	else if (renderID == -2) //bullet
	{
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textures[10].textureID);
		//
		//glActiveTexture(GL_TEXTURE0 + 1);
		//glBindTexture(GL_TEXTURE_2D, textures[8].textureID);
		//
		//glActiveTexture(GL_TEXTURE0 + 2);
		//glBindTexture(GL_TEXTURE_2D, textures[11].textureID);
		
	}
}

void ContentManager::renderBullet(int bid)
{
	switch (bid)
	{
	case BULLET_TYPE::PULSE_SHOT:
	case BULLET_TYPE::PLASMA_SHOT:
	case BULLET_TYPE::SHOTGUN_PELLET:
		trapperBullet.draw();
		break;
	case BULLET_TYPE::CLUSTER_GRENADE:
	case BULLET_TYPE::CLUSTERLING:
	case BULLET_TYPE::GRENADE_SHOT:
	case BULLET_TYPE::THERMITE_GRENADE:
		trapperConsume.draw();
		break;
	case BULLET_TYPE::DISC_SHOT:
		shankerBullet.draw();
		break;
	case BULLET_TYPE::HACKING_DART:
		shankerSpecial.draw();
	default:
		break;
	}
	
}

void ContentManager::renderThunderDome()
{
	bruteThunderDome.draw();
}

void ContentManager::renderPlayer(AnimManager::animState state, glm::mat4 world, GLuint uniformKeyMatrixLocation, bool first)
{
	playerCharacters[state.role].draw(uniformKeyMatrixLocation, state.state, state.frame, first);
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

	//Size is currently a vector containing the extremes for all of our world objects
	return size;
}

void ContentManager::bindLightwalTexture()
{
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[12].textureID);
}

void ContentManager::setRoomID(int room)
{
	testMap.currentChunk = room;
}