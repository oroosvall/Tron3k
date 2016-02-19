#include "ContentManager.h"
#include "..\Texture.h"

#include "../Utils/TimeQuery.h"

void ContentManager::init()
{
	//init flags
	f_portal_culling = true;
	f_freeze_portals = false;
	f_render_chunks = true;
	f_render_abb = false;
	f_render_obb = false;
	f_render_roombox = false;
	f_render_gui = true;

	tm.init();

	//blank_diffuse =		loadTexture("GameFiles/Textures/blank_d.dds");
	//blank_normal =		loadTexture("GameFiles/Textures/blank_n.dds");
	//blank_glow =		loadTexture("GameFiles/Textures/blank_g.dds");

	decalTexture = loadTexture("GameFiles/Textures/decal.png");
	//minimap
	int xx, yy;
	teamishereTexture = loadTexture("GameFiles/Textures/minimap/teamishere.png");
	youarehereTexture = loadTexture("GameFiles/Textures/minimap/youarehere.png", true, (int*)&xx, (int*)&yy);
	youareherescaleX = float(xx) / 1920;
	youareherescaleY = float(yy) / 1080;
	miniMapTexture = loadTexture("GameFiles/Textures/minimap/miniMap.png", true, (int*)&xx, (int*)&yy);
	minimapscaleX = float(xx) / 1920;
	minimapScaleY = float(yy) / 1080;
	mapBotcord = vec2(-185, 160);
	mapTopcord = vec2(165, -38);
	activeCapTexture = loadTexture("GameFiles/Textures/minimap/activecap.png", true, (int*)&xx, (int*)&yy);
	activeCapScale = vec2(float(xx) / 1920.0f, float(yy) / 1080.0f);

	//scoreboard
	score_team_orange = loadTexture("GameFiles/Textures/scoreboard/team_orange.png");
	score_team_green = loadTexture("GameFiles/Textures/scoreboard/team_green.png", true, (int*)&xx, (int*)&yy);
	score_teamscale = vec2(float(xx) / 1920, float(yy) / 1080);
	score_header_orange = loadTexture("GameFiles/Textures/scoreboard/header_orange.png");
	score_header_green = loadTexture("GameFiles/Textures/scoreboard/header_green.png", true, (int*)&xx, (int*)&yy);
	score_headerscale = vec2(float(xx) / 1920, float(yy) / 1080);

	skyTexture = 0;
	lightWallTex = 0;

	skyTexture = tm.createTexture("GameFiles/Textures/skybox.dds");
	lightWallTex = tm.createTexture("GameFiles/Textures/Blob.png");

	//addToStreamQueue(&skyTexture, "GameFiles/Textures/skybox.dds");
	//addToStreamQueue(&lightWallTex, "GameFiles/Textures/Blob.png");
		
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

	testMap.init(&tm);

	//count portals
	int nrofportals = 0;
	for (unsigned int n = 0; n < testMap.chunks.size(); n++)
		nrofportals += testMap.chunks[n].portals.size();

	//gen portals and assign them
	if (nrofportals > 0)
	{
		GLuint* querieIDS = new GLuint[nrofportals];
		glGenQueries(nrofportals, querieIDS);

		int id = 0;

		for (unsigned int n = 0; n < testMap.chunks.size(); n++)
			for (unsigned int k = 0; k < testMap.chunks[n].portals.size(); k++)
			{
				testMap.chunks[n].portals[k].query = querieIDS[id];
				id++;
			}
		delete [] querieIDS;
	}

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
}

void ContentManager::release()
{
	glDeleteTextures(1, &blank_diffuse);
	glDeleteTextures(1, &blank_normal);
	glDeleteTextures(1, &blank_glow);

	glDeleteTextures(1, &skyTexture);

	glDeleteTextures(1, &lightWallTex);
	glDeleteTextures(1, &decalTexture);

	glDeleteTextures(1, &miniMapTexture);
	glDeleteTextures(1, &youarehereTexture);
	glDeleteTextures(1, &teamishereTexture);

	glDeleteTextures(1, &score_team_green);
	glDeleteTextures(1, &score_team_orange);
	glDeleteTextures(1, &score_header_green);
	glDeleteTextures(1, &score_header_orange);

	testMap.release();

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
		if(testMap.chunks[c].roomOnlyRender.abbRender.size())
			testMap.chunks[c].roomOnlyRender.abbRender[0].abbBoxR.release();
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

	tm.release();

}

void ContentManager::update(float dt)
{
	tm.update(dt);
}

ContentManager::~ContentManager()
{
	
}

void ContentManager::renderChunks(GLuint shader, GLuint shaderLocation, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation, GLuint DglowColor, GLuint SglowColor, GLuint collision_portal_shader, GLuint collision_portal_world, GLuint portal_shader, GLuint portal_world)
{
	glUseProgram(shader);
	//diffuse
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[1].textureID);
	//normal & dynamic glow

	//glActiveTexture(GL_TEXTURE0 +1 );
	//glBindTexture(GL_TEXTURE_2D, textures[8].textureID);
	////static glow & spec
	//glActiveTexture(GL_TEXTURE0 + 2);
	//glBindTexture(GL_TEXTURE_2D, textures[6].textureID);

	//glProgramUniform1i(shader, textureLocation, 0);
	//glProgramUniform1i(shader, normalLocation, 1);
	//glProgramUniform1i(shader, glowSpecLocation, 2);

	for (int n = 0; n < nrChunks; n++)
	{
		renderedChunks[n] = false;
	}

	//render chunks logged from last frame
	for (int n = 0; n < nrChunks; n++)
	{
		if (renderNextChunks[n] || f_portal_culling == false)
		{
			//Glow values for world
			glProgramUniform3fv(shader, DglowColor, 1, (GLfloat*)&testMap.chunks[n].color[0]);
			glProgramUniform1f(shader, SglowColor, testMap.chunks[n].staticIntes);
			if (f_render_chunks)
				testMap.renderChunk(shader, shaderLocation, textureLocation, normalLocation, glowSpecLocation, n);
			renderedChunks[n] = true;
		}
	}

	glUseProgram(portal_shader);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	int portaltimer = startTimer("Portals");
	if (f_portal_culling)
	{
		for (int n = 0; n < nrChunks; n++)
			renderNextChunks[n] = false;

		renderNextChunks[0] = true;
		renderNextChunks[testMap.currentChunk] = true;

		PortalData* portal;
		for (int n = 0; n < nrChunks; n++)
		{
			if (renderedChunks[n] == true)
			{
				int size = testMap.chunks[n].portals.size();
				for (int p = 0; p < size; p++) // render the portals
				{
					portal = &testMap.chunks[n].portals[p];

					if (renderNextChunks[portal->bridgedRooms[0]] == false ||
						renderNextChunks[portal->bridgedRooms[1]] == false)
					{
						if (portal->waiting == false)
						{
							//dont render if it bridges between chunks that are already in the rendernextqueue
							portal->render(portal_shader, portal_world);
							portal->rendered = true;
							portal->waiting = true;
						}
						if (portal->rendered && portal->passedCulling())
						{
							renderNextChunks[portal->bridgedRooms[0]] = true;
							renderNextChunks[portal->bridgedRooms[1]] = true;
						}
						
					}
				}
			}
		}
	}
	stopTimer(portaltimer);

	if (f_render_abb || f_render_obb || f_render_roombox)
	{
		glUseProgram(collision_portal_shader);
		glm::mat4 alreadyinworldpace;
		glProgramUniformMatrix4fv(collision_portal_shader, collision_portal_world, 1, GL_FALSE, &alreadyinworldpace[0][0]);

		//render collision boxes
		for (int c = 0; c < nrChunks; c++)
		{
			ChunkCollision* col = testMap.chunks[c].getChunkCollision();

			int nrABB = col->abbStuff.size();
			for (int n = 0; n < nrABB - 1; n++)
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

			if (f_render_roombox)
			{
				if (c < nrChunks - 1)
				{
					testMap.chunks[c].roomOnlyRender.abbRender[0].abbBoxR.BindVertData();
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
				}
			}
		}
		testMap.renderCapAbb();
	}
}

void ContentManager::renderMisc(int renderID, GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation)
{
	if (renderID == -3) //skybox
	{
		glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, skyTexture);
		//
		//glActiveTexture(GL_TEXTURE0 + 1);
		//glBindTexture(GL_TEXTURE_2D, blank_normal);
		//
		//glActiveTexture(GL_TEXTURE0 + 2);
		//glBindTexture(GL_TEXTURE_2D, blank_glow);

		tm.bindTexture(skyTexture, shader, textureLocation, DIFFUSE_FB);
		tm.bindDefault(shader, normalLocation, NORMAL_FB);
		tm.bindDefault(shader, glowSpecLocation, GLOW_FB);

		glBindVertexArray(skybox.vao);
		glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ibo);

		glDrawElements(GL_TRIANGLES, skybox.faceCount * 3, GL_UNSIGNED_SHORT, 0);

		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);
	}
}

void ContentManager::renderCapturePoint(int id, GLuint shader, GLuint shaderLocation, GLuint diffuseLocation, GLuint normalLocation, GLuint glowLocation)
{

	testMap.renderCapturePoint(shader, shaderLocation, diffuseLocation, normalLocation, glowLocation, id);

}

void ContentManager::renderBullet(int bid)
{
	switch (bid)
	{
	case BULLET_TYPE::PULSE_SHOT:
	case BULLET_TYPE::PLASMA_SHOT:
	case BULLET_TYPE::SHOTGUN_PELLET:
	case BULLET_TYPE::BATTERY_SLOW_SHOT:
	case BULLET_TYPE::BATTERY_SPEED_SHOT:
	case BULLET_TYPE::LINK_SHOT:
		trapperBullet.draw();
		break;
	case BULLET_TYPE::CLUSTER_GRENADE:
	case BULLET_TYPE::CLUSTERLING:
	case BULLET_TYPE::GRENADE_SHOT:
	case BULLET_TYPE::THERMITE_GRENADE:
	case BULLET_TYPE::VACUUM_GRENADE:
	case BULLET_TYPE::GRAPPLING_HOOK:
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

void ContentManager::renderPlayer(AnimManager::animState state, glm::mat4 world, GLuint uniformKeyMatrixLocation, bool first, bool primary, GLuint shader, GLuint textureLocation, GLuint normalLocation, GLuint glowSpecLocation)
{
	playerCharacters[state.role].draw(uniformKeyMatrixLocation, state.state, state.frame, first, primary, shader, textureLocation, normalLocation, glowSpecLocation);
}

void* ContentManager::getChunkCollisionVectorAsPointer(int chunkID)
{
	if (chunkID < nrChunks)
		return (void*)testMap.getChunkCollision(chunkID);
	return nullptr;
}

void* ContentManager::getCapAsPointer(int& count)
{
	return testMap.getCapAsPointer(count);
}

void* ContentManager::getRoomBoxes()
{
	return testMap.getRoomBoxes();
}

std::vector<std::vector<float>> ContentManager::getMeshBoxes()
{
	std::vector<std::vector<float>> size;

	//Size is currently a vector containing the extremes for all of our world objects
	return size;
}

void ContentManager::bindLightwalTexture(GLuint shader, GLuint location)
{
	tm.bindTexture(lightWallTex, shader, location, DIFFUSE_FB);
}

void ContentManager::bindDecalTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, decalTexture);
}

void ContentManager::setRoomID(int room)
{
	testMap.currentChunk = room;
}