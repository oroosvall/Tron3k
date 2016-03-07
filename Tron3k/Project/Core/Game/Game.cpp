#include "Game.h"

void Game::release()
{
	// delete code goes here
	if (gamemode != nullptr)
		delete gamemode;
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i])
		{
			delete playerList[i];
		}
	}
	delete[]playerList;

	for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
	{
		for (unsigned int i = 0; i < bullets[c].size(); i++)
		{
			if (bullets[c][i] != nullptr)
			{
				delete bullets[c][i];
			}
		}
	}

	for (unsigned int c = 0; c < EFFECT_TYPE::NROFEFFECTS; c++)
	{
		for (unsigned int i = 0; i < effects[c].size(); i++)
		{
			if (effects[c][i] != nullptr)
			{
				delete effects[c][i];
			}
		}
	}

	for (size_t i = 0; i < allMovableParticles.size(); i++)
	{
		delete allMovableParticles[i];
	}

	physics->release();

	delete templateRole;
	delete this; // yes this is safe
}

Game::Game()
{

}

void Game::init(int max_connections, int state, Console* con)
{

	console = con;
	max_con = max_connections;

	loadRoles();
	initPhysics();
	if (GetSoundActivated())
	{
		GetSound()->playMapSounds();
	}

	gameState = state;

	gamemode = new KingOfTheHill();
	gamemode->init(con, this);

	playerList = new Player*[max_con];
	for (int c = 0; c < max_con; c++)
		playerList[c] = nullptr;

	freecam = false;
	spectateID = -1;
	decalCounter = 0;

	addEffectToList(0, 0, 0, EFFECT_TYPE::HSCPICKUP, vec3(50.87f, 1.5f, 10.7f), 0, 3.0f);
	addEffectToList(0, 0, 1, EFFECT_TYPE::HSCPICKUP, vec3(-81.8f, 1.5f, 45.85f), 0, 3.0f);
	addEffectToList(0, 0, 2, EFFECT_TYPE::HSCPICKUP, vec3(-34.86f, 1.5f, 67.0f), 0, 3.0f);
	addEffectToList(0, 0, 3, EFFECT_TYPE::HSCPICKUP, vec3(44.05f, 1.5f, 98.19f), 0, 3.0f);

	addEffectToList(0, 0, 0, EFFECT_TYPE::DOUBLEDAMAGEPICKUP, vec3(-4.6, 1.5, 69.23), 0, 4.0f);

	suicideMessages.push_back(" gave up on life.");
	suicideMessages.push_back(" short circuited!");
	suicideMessages.push_back(" had a battery attack!");
	suicideMessages.push_back(" took a swim!");
	suicideMessages.push_back(" is swimming with the fishes.");
	suicideMessages.push_back(" didn't want to play anymore.");
	suicideMessages.push_back(" pulled their own plug.");
	suicideMessages.push_back(" expired.");

	fragMessages.push_back(" was fragged by ");
	fragMessages.push_back(" was shredded by ");
	fragMessages.push_back(" was served a neon breakfast by ");
	fragMessages.push_back(" was stopped in their tracks by ");
	fragMessages.push_back(" said hi to ");
	fragMessages.push_back(" was disassembled by ");
	fragMessages.push_back(" was discontinued by ");
	fragMessages.push_back(" was short-circuited by ");
	fragMessages.push_back(" was recycled by ");
	fragMessages.push_back(" is in robot hell thanks to ");
	fragMessages.push_back(" was scrapped for parts by ");
}

void Game::loadRoles()
{
	string roles[NROFROLES];
	roles[TRAPPER] = "GameFiles/Roles/trapper.txt";
	roles[DESTROYER] = "GameFiles/Roles/destroyer.txt";
	roles[MOBILITY] = "GameFiles/Roles/mobility.txt";
	roles[BRUTE] = "GameFiles/Roles/brute.txt";
	roles[MANIPULATOR] = "GameFiles/Roles/manipulator.txt";

	ifstream roleFile;
	for (int i = 0; i < NROFROLES; i++)
	{
		for (int y = 0; y < NROFREADPROPERTIES; y += 5)
		{
			roleFile.open(roles[i]);

			getline(roleFile, loadedRoles[i][y]);
			getline(roleFile, loadedRoles[i][y + 1]);
			getline(roleFile, loadedRoles[i][y + 2]);
			getline(roleFile, loadedRoles[i][y + 3]);
			getline(roleFile, loadedRoles[i][y + 4]);
			roleFile.close();
		}
	}
	templateRole = new Role(loadedRoles);
}

void Game::initPhysics()
{
	physics = CreatePhysics();
	if (!physics->init())
	{
		//console.printMsg("Error: Pipeline could not be created!", "System", 'S');
		physics->release();
		physics = nullptr;
	}
	else
	{

		/*PipelineValues pv;
		pv.type = pv.INT2;
		pv.xy[0] = winX;
		pv.xy[1] = winY;
		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
		{
			console.printMsg("Error: Failed to set pipeline setting: VIEWPORT", "System", 'S');
		}*/
	}
}

int Game::nrOfPlayersReady()
{
	int ready = 0;
	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
		{
			if (playerList[c]->isReady())
				ready++;
		}
	}
	return ready;
}

void Game::nrOfPlayersReadyReset()
{
	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
			playerList[c]->unReady();
	}
}

vector<int>* Game::getTeamConIds(int team)
{
	if (team == 1)
		return &teamOne;
	else if (team == 2)
		return &teamTwo;
	return &teamSpectators;
}

void Game::update(float dt)
{
	//some things need to be done before movement, some after

	for (size_t i = 0; i < allMovableParticles.size(); i++)
	{
		if (allMovableParticles[i]->allowRemove)
		{
			delete allMovableParticles[i];
			allMovableParticles.erase(allMovableParticles.begin() + i);
		}
	}

	if (GetSoundActivated() && musicVolumeForMenu > 0 && GetSound()->getFading())
	{
		musicVolumeForMenu--;
		GetSound()->SetMenuVolume(musicVolumeForMenu);
	}

	if (gamemode->getType() == GAMEMODE_TYPE::KOTH && gameState == Gamestate::SERVER)
	{
		KingOfTheHill* k = (KingOfTheHill*)gamemode;
		k->clearPlayersOnCapPoint();
		for (int c = 0; c < 20; c++)
		{
			if (playerList[c] != nullptr)
			{
				if (playerList[c]->isAlive())
				{
					if (physics->checkPlayerVCaptureCollision(playerList[c]->getPos(), k->getCapturePoint()))
					{
						k->playerOnCapPointThisFrame(playerList[c]->getTeam());
					}
				}
			}
		}
	}

	GAMEMODE_MSG gameMsg = gamemode->update(dt);

	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
		{
			playerUpdate(c, dt);
			if (c == localPlayerId)
				playerApplyForces(c, dt);

			//TODO: Send collision results to player
		}
	}



	for (unsigned int i = 0; i < BULLET_TYPE::NROFBULLETS; i++)
	{
		for (unsigned int c = 0; c < bullets[i].size(); c++)
		{
			int msg = bullets[i][c]->update(dt);
			if (msg == 1)		//Bullet is dead
			{
				if (gameState == Gamestate::SERVER || gameState == Gamestate::ROAM)
				{
					BulletTimeOutInfo toi;
					toi.bt = BULLET_TYPE(i);
					int pid = -1, bid = -1;
					bullets[i][c]->getId(pid, bid);
					toi.bulletBID = bid;
					toi.bulletPID = pid;
					toi.pos = bullets[i][c]->getPos();
					allBulletTimeOuts.push_back(toi);
				}
			}
		}
	}

	updateDecals(dt);

	for (unsigned int i = 0; i < EFFECT_TYPE::NROFEFFECTS; i++)
	{
		for (unsigned int c = 0; c < effects[i].size(); c++)
		{
			int msg = effects[i][c]->update(dt);
			int pid = -1, eid = -1;
			effects[i][c]->getId(pid, eid);

			if (i == EFFECT_TYPE::LIGHT_WALL)
			{
				updateEffectBox(effects[i][c]);
			}

			if (msg == 1)		//Effect is dead
			{
				//effect removal in physics
				if (gameState == Gamestate::SERVER || gameState == Gamestate::ROAM)
				{
					EffectTimeOutInfo toi;
					toi.et = EFFECT_TYPE(i);
					toi.effectID = eid;
					toi.effectPID = pid;
					toi.pos = effects[i][c]->getPos();
					allEffectTimeOuts.push_back(toi);
				}
			}
			if (effects[i][c]->desynced())
			{
				int arraypos = -1;
				getSpecificEffect(pid, eid, EFFECT_TYPE(i), arraypos);
				removeEffect(EFFECT_TYPE(i), arraypos);
			}
		}
	}

	if (gameState == Gamestate::ROAM)
	{
		checkEffectVEffectCollision();
		checkPlayerVWorldCollision(dt);
		checkBulletVEffectCollision(dt);
		checkBulletVWorldCollision(dt);
		checkPlayerVEffectCollision();
		checkPlayerVCapCollision();
	}
	else if (gameState == Gamestate::CLIENT)
	{
		checkPvPCollision();
		checkPlayerVWorldCollision(dt);
		checkPlayerVEffectCollision();
		checkFootsteps(dt);
	}
	else if (gameState == Gamestate::SERVER)
	{
		checkPlayerVBulletCollision(dt);
		checkPlayerVCapCollision();
		checkBulletVWorldCollision(dt);
		checkPlayerVEffectCollision();
		checkEffectVEffectCollision();
		checkBulletVEffectCollision(dt);
	}

	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
		{
			bool spectatingThis = false;
			bool spectating = false;
			if (playerList[c]->isLocal())
			{

				if (spectateID > -1)
				{
					spectating = true;
					if (c == spectateID)
						spectatingThis = true;
				}
			}
			playerList[c]->movementUpdates(dt, freecam, spectatingThis, spectating);

			//TODO: Send collision results to player
		}
	}
	lastDT = dt;

}

void Game::playerUpdate(int conid, float dt)
{
	bool spectatingThis = false;
	bool spectating = false;
	if (spectateID > -1)
	{
		spectating = true;
		if (conid == spectateID)
			spectatingThis = true;
	}

	// apply movement vel and then handle collision
	PLAYERMSG msg = playerList[conid]->update(dt, freecam, spectatingThis, spectating);

	if (gameState != Gamestate::CLIENT)
	{
		if (playerList[conid]->allahuAkhbar())
		{
			BulletHitPlayerInfo suicide;
			suicide.bt = BULLET_TYPE::KILLYOURSELF;
			suicide.bulletBID = 0;
			suicide.bulletPID = 0;
			suicide.newHPtotal = 0;
			suicide.playerHit = conid;
			allBulletHitsOnPlayers.push_back(suicide);
		}
	}

	if (msg == PLAYERMSG::SHOOT)
	{
		registerWeapon(playerList[conid]);
	}
	if (msg == PLAYERMSG::WPNSWITCH)
	{
		registerSwitch(playerList[conid]);
	}
	if (msg == PLAYERMSG::SPECIALUSE)
	{
		registerSpecial(playerList[conid]);
	}
	if (msg == PLAYERMSG::MOBILITYUSE)
	{
		registerMobility(playerList[conid]);
	}
	if (msg == PLAYERMSG::USEITEM)
	{
		registerConsumable(playerList[conid]);
	}
	if (msg == PLAYERMSG::DEATH)
	{
		//freecam = true;
	}
	if (msg == PLAYERMSG::PLAYERRESPAWN)
	{
		if (playerList[conid]->isLocal())
			localPlayerWantsRespawn = true;
	}
}

void Game::playerApplyForces(int conid, float dt)
{
	if (playerList[conid]->getPos().y > 20.0f)
		playerList[conid]->setVelocity(vec3(playerList[conid]->getVelocity().x, 0.0f, playerList[conid]->getVelocity().z));
	playerList[conid]->applyGravity(physics->addGravity(dt));
}

Player* Game::getPlayer(int conID)
{
	if (conID < max_con)
		return playerList[conID];
	return nullptr;
}

std::vector<Bullet*> Game::getBullets(BULLET_TYPE type)
{
	return bullets[type];
}

std::vector<Effect*> Game::getEffects(EFFECT_TYPE type)
{
	return effects[type];
}

void Game::createPlayer(Player* p, int conID, int hp, int role, bool isLocal)
{
	if (getGameMode()->getType() == GAMEMODE_TYPE::KOTH)
	{
		KingOfTheHill* k = (KingOfTheHill*)getGameMode();
		if (k->getState() == WARMUP)
			console->printMsg("Warmup. Type /ready to start.", "System", 'S');
	}
	playerList[conID] = new Player();
	playerList[conID]->init(p->getName(), p->getPos(), isLocal);
	playerList[conID]->setTeam(p->getTeam());
	playerList[conID]->setHP(hp);
	playerList[conID]->setRole(*templateRole);
	if (role != ROLES::NROFROLES)
		playerList[conID]->chooseRole(role);

	if (isLocal)
	{
		localPlayerId = conID;
		freecam = true;
	}
}

void Game::removePlayer(int conID)
{
	delete playerList[conID];
	playerList[conID] = nullptr;
	removeConIDfromTeams(conID);
}

void Game::sendChunkBoxes(int chunkID, void* cBoxes)
{
	physics->receiveChunkBoxes(chunkID, cBoxes);
}

void Game::sendCapBoxes(int nrCaps, void* capBoxes)
{
	//oh capbox my capbox
	physics->receiveCap(nrCaps, capBoxes);
}

void Game::sendRoomBoxes(void* roomboxes)
{
	physics->receiveRoomBoxes(roomboxes);
}

void Game::sendPlayerBox(std::vector<float> pBox)
{
	physics->receivePlayerBox(pBox, 0.9f);
}

void Game::sendPlayerRadSize(float rad)
{
	physics->receivePlayerRad(rad);
}

void Game::sendWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	int x = 0;
	//physics->receiveWorldBoxes(wBoxes);
}

void Game::updateEffectBox(Effect* effect)
{
	std::vector<float> eBox;
	switch (effect->getType())
	{
		int pid, eid;
	case EFFECT_TYPE::LIGHT_WALL:

		if (((LightwallEffect*)(effect))->getDong()) //since the effect updates, we need to resend the box to physics
		{
			((LightwallEffect*)(effect))->getId(pid, eid);
			physics->removeEffect(eid, pid, EFFECT_TYPE::LIGHT_WALL);
			glm::vec3 sPos, gPos;
			float height = 1.8f;

			gPos = ((LightwallEffect*)(effect))->getEndPoint();
			sPos = ((LightwallEffect*)(effect))->getPos();
			std::vector<float> eBox;
			eBox.push_back(sPos.x);
			eBox.push_back(sPos.y);
			eBox.push_back(sPos.z);
			eBox.push_back(height);
			eBox.push_back(gPos.x);
			eBox.push_back(gPos.y);
			eBox.push_back(gPos.z);
			physics->receiveEffectBox(eBox, EFFECT_TYPE::LIGHT_WALL, pid, eid);
		}
		break;
	case EFFECT_TYPE::THUNDER_DOME:

		break;
	case EFFECT_TYPE::BATTERY_SLOW:

		break;
	case EFFECT_TYPE::BATTERY_SPEED:

		break;
	case EFFECT_TYPE::THERMITE_CLOUD:

		break;
	case EFFECT_TYPE::VACUUM:

		break;
	case EFFECT_TYPE::EXPLOSION:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, EFFECT_TYPE::EXPLOSION, pid, eid);
		break;
	}
}

void Game::checkFootsteps(float dt)
{
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i] != nullptr && i != localPlayerId)
		{
			if (!playerList[i]->getFootsteps())
			{
				playerList[i]->footstepsLoopReset(dt);
			}

			if (playerList[i]->getFootsteps() && (playerList[i]->getAnimState_t_p() != AnimationState::third_primary_air))
			{
				glm::vec3 pos;
				glm::vec3 vel;
				pos = playerList[i]->getPos();
				vel = playerList[i]->getVelocity();
				if (vel.x > 0.5 || vel.x < -0.5 || vel.z > 0.5 || vel.z < -0.5)
				{
					playerList[i]->setFootstepsCountdown();
					playerList[i]->setFootstepsLoop(false);
					if (GetSoundActivated())
					{
						if (i == spectateID)
						{
							GetSound()->PlayStereoFootsteps(playerList[i]->getRole()->getRole(), pos.x, pos.y, pos.z);
						}
						else
						{
							GetSound()->playFootsteps(playerList[i]->getRole()->getRole(), pos.x, pos.y, pos.z);
						}
					}

				}

			}

			if (!playerList[i]->CheckAbleToJumpSound())
			{
				playerList[i]->CoolDownJump(dt);
			}

			if (!playerList[i]->CheckAbleToJumpSound() && (playerList[i]->getAnimState_t_p() != AnimationState::third_primary_air))
			{
				playerList[i]->jumpSoundAble = true;
			}

			if (playerList[i]->CheckAbleToJumpSound() && (playerList[i]->getAnimState_t_p() == AnimationState::third_primary_air))
			{
				glm::vec3 pos;
				pos = playerList[i]->getPos();
				playerList[i]->SetJumpCoolDown(100.0f);

				if (GetSoundActivated())
				{
					if (i == spectateID)
					{
						GetSound()->PlayStereoJump(playerList[i]->getRole()->getRole());
					}
					else
					{
						GetSound()->playJump(playerList[i]->getRole()->getRole(), pos.x, pos.y, pos.z);
					}
				}



			}
		}
	}
}

int Game::getMaxCon()
{
	return this->max_con;
}

Player** Game::getPlayerList()
{
	return this->playerList;
}

void Game::checkPvPCollision()
{
	//TEMPORARY
	vec3 pPos[20];
	vec3 localPPos = vec3(0, 0, 0);
	int localP = -1;
	int localPTeam = -1;
	for (int i = 0; i < max_con && localPPos == vec3(0, 0, 0); i++)
	{
		if (playerList[i] != nullptr)
		{
			if (!playerList[i]->isLocal())
				pPos[i] = playerList[i]->getPos();
			else
			{
				localPPos = playerList[i]->getPos();
				localP = i;
				localPTeam = playerList[i]->getTeam();
			}
		}
		else
		{

		}
	}

	//Checks collision between players
	//TODO: improve so it just checks between ourselves and all other players, we don't care if two other players collided
	glm::vec3 collides = glm::vec3(0, 0, 0);
	for (int i = 0; i < max_con; i++)
	{
		collides = glm::vec3(0, 0, 0);
		if (playerList[i] != nullptr)
			collides = physics->checkPlayerVPlayerCollision(localPPos, pPos[i]);

		if (collides != glm::vec3(0, 0, 0))
		{

			//here we can do things when two objects, collide, cause now we know i and j collided.
			//int x = 0;
			//TODO: Add separated collision places for friendly vs hostile
			if (playerList[i]->getTeam() == localPTeam)
			{
				//Friendly

			}
			else
			{
				//Enemy

			}

		}

		//TODO: Add bullet collision

	}

}

void Game::checkPlayerVEffectCollision()
{
	glm::vec4 collNormals = vec4(0, 0, 0, 0);
	bool cold = false;
	if (gameState == Gamestate::ROAM || gameState == Gamestate::CLIENT)
	{
		Player* local = playerList[localPlayerId];
		//Collision for all wall-like effects i.e. only Lightwall and Thunderdome

		glm::vec4 collNormalWalls;
		glm::vec4 collNormalDomes;

		int size = (int)effects[EFFECT_TYPE::LIGHT_WALL].size();
		for (int c = 0; c < size; c++)
		{
			int eid = -1, pid = -1;
			effects[EFFECT_TYPE::LIGHT_WALL][c]->getId(pid, eid);
			if (((LightwallEffect*)effects[EFFECT_TYPE::LIGHT_WALL][c])->getCollidable() || localPlayerId != pid)
			{
				collNormalWalls = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::LIGHT_WALL, eid, cold);
				if (cold)
					if (collNormalWalls != vec4(0, 0, 0, 0))
						playerList[localPlayerId]->addCollisionNormal(collNormalWalls);
			}

		}
		size = (int)effects[EFFECT_TYPE::THUNDER_DOME].size();
		for (int c = 0; c < size; c++)
		{
			int eid = -1, pid = -1;
			effects[EFFECT_TYPE::THUNDER_DOME][c]->getId(pid, eid);
			collNormalDomes = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::THUNDER_DOME, eid, cold);
			if (cold)
				if (collNormalDomes != vec4(0, 0, 0, 0))
					playerList[localPlayerId]->addCollisionNormal(collNormalDomes);
			//this is to be changed, we need to calculate a proper normal for the dome

			//vec3 n = vec3(collNormalDomes[0]);

			//n = normalize(n);
		}

		/*//merge normals into one vector
		collNormals.reserve(collNormalWalls.size() + collNormalDomes.size()); // preallocate memory
		collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
		collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());*/
	}
	if (gameState == Gamestate::ROAM || gameState == Gamestate::SERVER)
	{
		//Collision for all non-wall effects
		bool notDoingWrongThings = true;
		for (int j = 0; j < max_con; j++)
		{
			if (playerList[j] != nullptr)
			{
				for (int t = EFFECT_TYPE::EXPLOSION; t < EFFECT_TYPE::NROFEFFECTS; t++)
				{
					int effectsSize = (int)effects[t].size();
					for (int i = 0; i < effectsSize; i++)
					{
						int eid = -1, pid = -1;
						effects[t][i]->getId(pid, eid);
						if (!effects[t][i]->thisPlayerHasBeenHitByMe(j))
						{
							if (effects[t][i]->getTeam() != playerList[j]->getTeam())
							{
								collNormals = physics->checkPlayerVEffectCollision(playerList[j]->getPos(), t, eid, cold);
								if (cold)
									if (collNormals != vec4(0, 0, 0, 0))
									{
										if (effects[t][i]->getType() == EFFECT_TYPE::HSCPICKUP || effects[t][i]->getType() == EFFECT_TYPE::DOUBLEDAMAGEPICKUP)
										{
											if (effects[t][i]->getType() == EFFECT_TYPE::HSCPICKUP)
											{
												HSCPickup* temp = (HSCPickup*)effects[t][i];
												if (temp->onCooldown())
												{
													notDoingWrongThings = false;
												}
											}
											if (effects[t][i]->getType() == EFFECT_TYPE::DOUBLEDAMAGEPICKUP)
											{
												DoubleDamagePickup* temp = (DoubleDamagePickup*)effects[t][i];
												if (temp->onCooldown())
												{
													notDoingWrongThings = false;
												}
											}
										}
										if (notDoingWrongThings)
										{
											effects[t][i]->thisPlayerHit(j);
											EffectHitPlayerInfo hi;
											hi.playerHit = j;
											hi.effectPID = pid;
											hi.effectID = eid;
											hi.et = EFFECT_TYPE(t);
											hi.hitPos = effects[t][i]->getPos();
											hi.playerPos = playerList[j]->getPos();
											hi.newHPtotal = -1;
											allEffectHitsOnPlayers.push_back(hi);
										}
									}
							}
						}
					}
				}
			}
		}
	}
}

void Game::checkPlayerVCapCollision()
{
	if (gameState == Gamestate::ROAM)
	{
		bool localCollided = physics->checkPlayerVCaptureCollision(playerList[localPlayerId]->getPos(), 0);
	}
	else if (gameState == Gamestate::SERVER)
	{
		bool inPoint = false;

		for (int i = 0; i < max_con; i++)
		{
			if (playerList[i] != nullptr)
			{
				inPoint = physics->checkPlayerVCaptureCollision(playerList[i]->getPos(), 0);
			}
		}
	}
}

void Game::checkPlayerVBulletCollision(float dt)
{
	glm::vec3 collides = glm::vec3(0, 0, 0);

	for (int i = 0; i < max_con; i++)
	{
		Player* p = playerList[i];
		if (p != nullptr)
		{
			for (unsigned int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
			{
				if (b != BULLET_TYPE::CLUSTERLING && b != BULLET_TYPE::BATTERY_SLOW_SHOT && b != BULLET_TYPE::BATTERY_SPEED_SHOT && b != BULLET_TYPE::BATTERY_SLOW_SHOT)
				{
					for (unsigned int j = 0; j < bullets[b].size(); j++)
					{
						collides = glm::vec3(0, 0, 0);
						if (bullets[b][j] != nullptr)
						{
							int pid = -1, bid = -1;
							bullets[b][j]->getId(pid, bid);
							if ((bullets[b][j]->getTeam() != playerList[i]->getTeam() && playerList[i]->getTeam() != 0) || (playerList[pid]->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER) && pid != i)) //Don't shoot same team, don't shoot spectators
							{
								if (playerList[i]->isAlive()) //Don't shoot dead people
								{
									float modifier = 1.0f;
									if (b == BULLET_TYPE::GRENADE_SHOT || b == BULLET_TYPE::CLUSTERLING)
										modifier = 2.5f;
									if (b == BULLET_TYPE::MELEE_ATTACK)
										modifier = 1.5f;
									bool col = false;
									collides = physics->checkPlayerVBulletCollision(playerList[i]->getPos() - (vec3(0, playerList[i]->getRole()->getBoxModifier(), 0)), bullets[b][j]->getPos(), this->playerList[i]->getRole()->GetSize(), bullets[b][j]->getDir(), bullets[b][j]->getVel(), dt, col, modifier);
									if (col)
									{
										if (collides != glm::vec3(0, 0, 0))
										{
											//Code for player on bullet collision goes here
											/*

											Spara undan player som blev träffad (dvs i), bulletId (pID och bID) och BULLET_TYPE. Även dir.
											Lägg förslagsvis i en vector<struct> som kan skickas till Core. Core matar in alla träffade spelare i ett NET_EVENT (conID, bID, pID, bt).
											NET_EVENT eftersom allt detta sker endast på servern
											Tas emot av klienterna, Core->Game. Game hanterar sina bullethits när paketet tas emot.

											Servern gör givetvis samma beräkning för bullethits (speed/position-data pga knockbacks antingen görs inte eller görs och sedan discardas pga client->server update)
											direkt när kollisionen sker, dvs.

											*/
											BulletHitPlayerInfo hit;
											hit.bt = BULLET_TYPE(b);
											hit.playerHit = i;
											bullets[b][j]->getId(hit.bulletPID, hit.bulletBID);
											hit.newHPtotal = -1;
											hit.hitDir = bullets[b][j]->getDir();
											allBulletHitsOnPlayers.push_back(hit);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Game::checkPlayerVWorldCollision(float dt)
{
	if (localPlayerId != -1)
	{
		vec3 posadjust = vec3(0);
		//lower with distance from eyes to center
		std::vector<vec4> cNorms = physics->PlayerVWorldCollision(playerList[localPlayerId]->getPos() - vec3(0, playerList[localPlayerId]->getRole()->getBoxModifier(), 0), playerList[localPlayerId]->getDir(), playerList[localPlayerId]->getVelocity(), dt);


		//if we collided with something
		if (cNorms.size() > 0)
		{
			playerList[localPlayerId]->setCollisionInfo(cNorms);

			//Rest is handled by player
		}
	}
}

void Game::checkBulletVWorldCollision(float dt)
{
	glm::vec4 collides = vec4(-1, -1, -1, -1);

	for (unsigned int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
	{
		for (unsigned int j = 0; j < bullets[b].size(); j++)
		{
			if (bullets[b][j] != nullptr)
			{
				if (!bullets[b][j]->getCollidedThisFrame())
				{
					int pid = -1, bid = -1;
					bullets[b][j]->getId(pid, bid);
					vec3 bPos = bullets[b][j]->getPos();
					collides = physics->BulletVWorldCollision(bPos, bullets[b][j]->getVel(), bullets[b][j]->getDir(), dt);
					if (collides.w > -1 || collides.w < -1)
					{
						BulletHitWorldInfo hi;
						hi.bt = BULLET_TYPE(b); hi.hitPos = bPos; hi.hitDir = bullets[b][j]->getDir();
						bullets[b][j]->getId(hi.bulletPID, hi.bulletBID);
						hi.collisionNormal = collides;
						allBulletHitsOnWorld.push_back(hi);
					}
				}
			}
		}
	}
}

void Game::checkBulletVEffectCollision(float dt)
{
	glm::vec4 collNormals = vec4(0, 0, 0, 0);
	if (gameState == Gamestate::ROAM || gameState == Gamestate::SERVER)
	{
		//Collision for all wall-like effects i.e. only Lightwall and Thunderdome

		glm::vec4 collNormalWalls = vec4(0, 0, 0, 0);
		glm::vec4 collNormalDomes = vec4(0, 0, 0, 0);

		for (unsigned int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
		{
			for (unsigned int j = 0; j < bullets[b].size(); j++)
			{
				for (int c = 0; c < effects[EFFECT_TYPE::LIGHT_WALL].size(); c++)
				{
					int eid = -1, pid = -1;
					effects[EFFECT_TYPE::LIGHT_WALL][c]->getId(pid, eid);
					if (((LightwallEffect*)effects[EFFECT_TYPE::LIGHT_WALL][c])->getCollidable())
					{
						vec3 bPos = bullets[b][j]->getPos();
						collNormalWalls = physics->checkBulletVEffectCollision(bPos, bullets[b][j]->getVel(), bullets[b][j]->getDir(), EFFECT_TYPE::LIGHT_WALL, eid, dt);
						if (collNormalWalls != vec4(0, 0, 0, 0))
						{
							BulletHitEffectInfo bi;
							bullets[b][j]->getId(bi.bulletPID, bi.bulletBID);
							bi.bt = BULLET_TYPE(b);
							bi.effectID = eid;
							bi.effectPID = pid;
							bi.bulletTeam = bullets[b][j]->getTeam();
							bi.et = EFFECT_TYPE::LIGHT_WALL;
							bi.hitPos = bPos;
							bi.hitDir = bullets[b][j]->getDir();
							bi.collisionNormal = collNormalWalls;
							allBulletHitsOnEffects.push_back(bi);
						}
					}
				}

				for (int c = 0; c < effects[EFFECT_TYPE::THUNDER_DOME].size(); c++)
				{
					int eid = -1, pid = -1;
					effects[EFFECT_TYPE::THUNDER_DOME][c]->getId(pid, eid);
					vec3 bPos = bullets[b][j]->getPos();
					collNormalDomes = physics->checkBulletVEffectCollision(bPos, bullets[b][j]->getVel(), bullets[b][j]->getDir(), EFFECT_TYPE::THUNDER_DOME, eid, dt);
					if (collNormalDomes != vec4(0, 0, 0, 0))
					{
						int x = 0;
						BulletHitEffectInfo bi;
						bullets[b][j]->getId(bi.bulletPID, bi.bulletBID);
						bi.bt = BULLET_TYPE(b);
						bi.effectID = eid;
						bi.effectPID = pid;
						bi.bulletTeam = bullets[b][j]->getTeam();
						bi.et = EFFECT_TYPE::THUNDER_DOME;
						bi.hitPos = bPos;
						bi.hitDir = bullets[b][j]->getDir();
						bi.collisionNormal = collNormalDomes;
						allBulletHitsOnEffects.push_back(bi);
					}
				}

				if (BULLET_TYPE(b) == BULLET_TYPE::BATTERY_SLOW_SHOT || BULLET_TYPE(b) == BULLET_TYPE::BATTERY_SPEED_SHOT || BULLET_TYPE(b) == BULLET_TYPE::FUSION_SHOT)
				{
					for (int c = 0; c < effects[EFFECT_TYPE::BATTERY_SLOW].size(); c++)
					{
						int eid = -1, pid = -1;
						effects[EFFECT_TYPE::BATTERY_SLOW][c]->getId(pid, eid);
						vec3 bPos = bullets[b][j]->getPos();
						collNormalWalls = physics->checkBulletVEffectCollision(bPos, bullets[b][j]->getVel(), bullets[b][j]->getDir(), EFFECT_TYPE::BATTERY_SLOW, eid, dt);
						if (collNormalWalls != vec4(0, 0, 0, 0))
						{
							bullets[b][j]->collidedThisFrame();
							BulletHitEffectInfo bi;
							bullets[b][j]->getId(bi.bulletPID, bi.bulletBID);
							bi.bt = BULLET_TYPE(b);
							bi.effectID = eid;
							bi.effectPID = pid;
							bi.bulletTeam = bullets[b][j]->getTeam();
							bi.et = EFFECT_TYPE::BATTERY_SLOW;
							bi.hitPos = bPos;
							bi.hitDir = bullets[b][j]->getDir();
							bi.collisionNormal = collNormalWalls;
							allBulletHitsOnEffects.push_back(bi);
						}
					}

					for (int c = 0; c < effects[EFFECT_TYPE::BATTERY_SPEED].size(); c++)
					{
						int eid = -1, pid = -1;
						effects[EFFECT_TYPE::BATTERY_SPEED][c]->getId(pid, eid);
						vec3 bPos = bullets[b][j]->getPos();
						collNormalWalls = physics->checkBulletVEffectCollision(bPos, bullets[b][j]->getVel(), bullets[b][j]->getDir(), EFFECT_TYPE::BATTERY_SPEED, eid, dt);
						if (collNormalWalls != vec4(0, 0, 0, 0))
						{
							bullets[b][j]->collidedThisFrame();
							BulletHitEffectInfo bi;
							bullets[b][j]->getId(bi.bulletPID, bi.bulletBID);
							bi.bt = BULLET_TYPE(b);
							bi.effectID = eid;
							bi.effectPID = pid;
							bi.bulletTeam = bullets[b][j]->getTeam();
							bi.et = EFFECT_TYPE::BATTERY_SPEED;
							bi.hitPos = bPos;
							bi.hitDir = bullets[b][j]->getDir();
							bi.collisionNormal = collNormalWalls;
							allBulletHitsOnEffects.push_back(bi);
						}
					}
				}
			}
		}

		/*//merge normals into one vector
		collNormals.reserve(collNormalWalls.size() + collNormalDomes.size()); // preallocate memory
		collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
		collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());*/

	}
	/*if (gameState == Gamestate::ROAM || gameState == Gamestate::SERVER)
	{
		//Collision for all non-wall effects
		std::vector<vec4> explosColls;
		collNormals.clear();
		for (int j = 0; j < max_con; j++)
		{
			if (playerList[j] != nullptr)
			{
				for (int i = 0; i < effects[EFFECT_TYPE::EXPLOSION].size(); i++)
				{
					int eid = -1, pid = -1;
					effects[EFFECT_TYPE::EXPLOSION][i]->getId(pid, eid);

					if (pid != j && playerList[pid]->getTeam() != playerList[j]->getTeam())
					{
						explosColls = physics->checkPlayerVEffectCollision(playerList[j]->getPos(), EFFECT_TYPE::EXPLOSION, eid);
						collNormals.reserve(explosColls.size()); // preallocate memory
						collNormals.insert(collNormals.end(), explosColls.begin(), explosColls.end());
						explosColls.clear();
					}
				}
				playerList[j]->setExplodingInfo(collNormals);
				collNormals.clear();
			}

		}
	}*/
}

void Game::checkEffectVEffectCollision()
{
	for (unsigned int i = 0; i < effects[EFFECT_TYPE::CLEANSENOVA].size(); i++)
	{
		int cleansepid = -1, cleanseeid = -1;
		effects[EFFECT_TYPE::CLEANSENOVA][i]->getId(cleansepid, cleanseeid);
		for (unsigned int t = 0; t < EFFECT_TYPE::NROFEFFECTS; t++)
		{
			if (t != EFFECT_TYPE::CLEANSENOVA)
			{
				for (unsigned int k = 0; k < effects[t].size(); k++)
				{
					int pid = -1, eid = -1;
					effects[t][k]->getId(pid, eid);
					bool e_coli = physics->checkEffectVEffectCollision(EFFECT_TYPE::CLEANSENOVA, EFFECT_TYPE(t), cleanseeid, eid, cleansepid, pid);
					if (e_coli)
					{
						EffectTimeOutInfo toi;
						toi.effectID = eid;
						toi.effectPID = pid;
						toi.et = EFFECT_TYPE(t);
						toi.pos = effects[t][k]->getPos();
						allEffectTimeOuts.push_back(toi);
					}
				}
			}
		}
	}


	//Use this line, etype1, eid1 and pid1 are for the thing, the 2s are for the other things, the lightwalls etc
	//so thing1 is the thing removing effects.

	//so just alter that line.
	//or don't
	//whatever.
	//It's not like I care if you do or anything



	//...


	//baka
}

void Game::registerWeapon(Player* p)
{
	Weapon* wpn = p->getPlayerCurrentWeapon();
	weaponShotWith = wpn->getType();
	bulletShot = wpn->getBulletId();
	shotsFired = true;
}

void Game::registerSwitch(Player* p)
{
	weaponSwitchedTo = p->getPlayerCurrentWeapon()->getType();
	wpnSwapLocation = p->getRole()->getWeaponPosition();
	wpnSwitched = true;
}

void Game::registerSpecial(Player* p)
{
	Special* s = p->getPlayerSpecialAbility();
	specialUsed = s->getType();
	specialActivated = true;
}

void Game::registerMobility(Player* p)
{
	Special* m = p->getRole()->getMobilityAbility();
	specialUsed = m->getType();
	specialActivated = true;
}

void Game::registerConsumable(Player* p)
{
	Consumable* c = p->getRole()->getConsumable();
	itemUsed = c->getType();
	consumableUsed = true;
}

void Game::handleWeaponSwitch(int conID, WEAPON_TYPE ws, int swapLoc)
{
	if (conID != localPlayerId)
		playerList[conID]->switchWpn(ws, swapLoc);
}

int Game::getPlayersOnTeam(int team)
{
	switch (team)
	{
	case 0: return teamSpectators.size();
	case 1: return teamOne.size();
	case 2: return teamTwo.size();
	}
	return 0;
}

void Game::removeConIDfromTeams(int conID)
{
	for (unsigned int c = 0; c < teamSpectators.size(); c++)
	{
		if (teamSpectators[c] == conID)
		{
			teamSpectators[c] = teamSpectators[teamSpectators.size() - 1];
			teamSpectators.pop_back();
			return;
		}
	}
	for (unsigned int c = 0; c < teamOne.size(); c++)
	{
		if (teamOne[c] == conID)
		{
			teamOne[c] = teamOne[teamOne.size() - 1];
			teamOne.pop_back();
			return;
		}
	}
	for (unsigned int c = 0; c < teamTwo.size(); c++)
	{
		if (teamTwo[c] == conID)
		{
			teamTwo[c] = teamTwo[teamTwo.size() - 1];
			teamTwo.pop_back();
			return;
		}
	}
}

void Game::addPlayerToTeam(int p_conID, int team)
{
	switch (team)
	{
	case 0:
		removeConIDfromTeams(p_conID);
		teamSpectators.push_back(p_conID);
		playerList[p_conID]->setTeam(0);
		break;
	case 1:
		removeConIDfromTeams(p_conID);
		teamOne.push_back(p_conID);
		playerList[p_conID]->setTeam(1);

		break;
	case 2:
		removeConIDfromTeams(p_conID);
		teamTwo.push_back(p_conID);
		playerList[p_conID]->setTeam(2);
		break;
	}
}

void Game::getLatestWeaponFired(int localPlayer, WEAPON_TYPE &wt, int &bulletId)
{
	shotsFired = false;
	Player* p = playerList[localPlayer];
	handleWeaponFire(localPlayer, p->getTeam(), bulletShot, weaponShotWith, p->getPos(), p->getDir());
	wt = weaponShotWith;
	bulletId = bulletShot;
	bulletShot = -1;
}

void Game::addBulletToList(int conID, int teamId, int bulletId, BULLET_TYPE bt, glm::vec3 pos, glm::vec3 dir)
{

	Bullet* b = nullptr;
	Player* p = playerList[conID];
	glm::vec3 rightV = normalize(cross(dir, vec3(0, 1, 0)));
	glm::vec3 upV = normalize(cross(rightV, dir));
	glm::vec3 dirMod = dir;
	switch (bt)
	{
	case BULLET_TYPE::PULSE_SHOT:
		rightV *= 0.2;
		upV *= -0.13f;
		dirMod *= 0.6f;
		pos += upV + rightV + dirMod;
		b = new PulseShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::PLASMA_SHOT:
		upV *= -0.17;
		dirMod *= 0.1f;
		pos += upV + dirMod;
		b = new PlasmaShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::GRENADE_SHOT:
		rightV *= -0.55;
		upV *= 0.2f;
		dirMod *= 0.35f;
		pos += upV + rightV + dirMod;
		b = new GrenadeShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::SHOTGUN_PELLET:
		rightV *= 0.4;
		upV *= -0.3f;
		dirMod *= 0.5f;
		pos += upV + rightV + dirMod;
		b = new ShotgunPellet(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::THERMITE_GRENADE:
		b = new ThermiteGrenade(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::CLEANSE_BOMB:
		b = new CleanseBomb(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::CLUSTER_GRENADE:
		rightV *= -0.25f;
		pos += rightV;
		b = new ClusterGrenade(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::CLUSTERLING:
		b = new Clusterling(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::BATTERY_SLOW_SHOT:
		rightV *= 0.25;
		upV *= -0.18f;
		dirMod *= 0.6f;
		pos += upV + rightV + dirMod;
		b = new BatterySlowShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::BATTERY_SPEED_SHOT:
		rightV *= -0.25;
		upV *= -0.18f;
		dirMod *= 0.6f;
		pos += upV + rightV + dirMod;
		b = new BatterySpeedShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::FUSION_SHOT:
		rightV *= 0.2;
		upV *= -0.15f;
		dirMod *= 0.8f;
		pos += upV + rightV + dirMod;
		b = new FusionShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::VACUUM_GRENADE:
		b = new VacuumGrenade(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::DISC_SHOT:
		rightV *= 0.25f;
		upV *= 0.2f;
		dirMod *= 0.65f;
		pos += upV + rightV + dirMod;
		b = new DiscShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::HACKING_DART:
		rightV *= 0.25f;
		upV *= 0.2f;
		dirMod *= 0.65f;
		pos += upV + rightV + dirMod;
		b = new HackingDart(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::MELEE_ATTACK:
		b = new MeleeAttack(vec3(999.0f, 999.0f, 999.0f), dir, conID, bulletId, teamId, p);
		break;
	case BULLET_TYPE::GRAPPLING_HOOK:
		rightV *= 0.2;
		upV *= -0.15f;
		dirMod *= 0.8f;
		pos += upV + rightV + dirMod;
		b = new GrapplingHook(pos, dir, conID, bulletId, teamId);
	}
	b->part = 0;
	b->part2 = 0;

	bullets[bt].push_back(b);


	if (gameState != SERVER)
	{
		MovableParticle* mp = new MovableParticle();
		mp->bt = BULLET_TYPE(b->getType());
		mp->id = 0;
		mp->created = false;
		mp->dead = false;
		mp->allowRemove = false;
		mp->pPos = b->getPosPtr();
		mp->dir = -b->getDir();
		if (teamId == 1)
			mp->color = TEAMONECOLOR;
		if (teamId == 2)
			mp->color = TEAMTWOCOLOR;

		b->part = mp;
		allMovableParticles.push_back(mp);

		if (mp->bt == BULLET_TYPE::FUSION_SHOT)
		{
			mp = new MovableParticle();
			mp->bt = BULLET_TYPE::FUSION_SHOT2;
			mp->id = 0;
			mp->created = false;
			mp->dead = false;
			mp->allowRemove = false;
			mp->pPos = b->getPosPtr();
			mp->dir = -b->getDir();
			if (teamId == 1)
				mp->color = TEAMONECOLOR;
			if (teamId == 2)
				mp->color = TEAMTWOCOLOR;

			b->part2 = mp;
			allMovableParticles.push_back(mp);
		}
		else if (mp->bt == BULLET_TYPE::PULSE_SHOT)
		{
			mp = new MovableParticle();
			mp->bt = BULLET_TYPE::PULSE_SHOT2;
			mp->id = 0;
			mp->created = false;
			mp->dead = false;
			mp->allowRemove = false;
			mp->pPos = b->getPosPtr();
			mp->dir = -b->getDir();
			if (teamId == 1)
				mp->color = TEAMONECOLOR;
			if (teamId == 2)
				mp->color = TEAMTWOCOLOR;

			b->part2 = mp;
			allMovableParticles.push_back(mp);
		}
	}
}

void Game::handleWeaponFire(int conID, int teamId, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir)
{
	switch (weapontype)
	{
	case WEAPON_TYPE::PULSE_RIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectPulseRifleShotStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z, CATEGORY::Guns);
			}

		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::PULSE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::ENERGY_BOOST:
		if (GetSoundActivated())
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectEnergyBoostStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectEnergyBoost, pos.x, pos.y, pos.z, CATEGORY::Guns);
			}
		playerList[conID]->healing(15);
		playerList[conID]->getRole()->swapWeapon(WEAPON_TYPE::ENERGY_BOOST, 0);
		break;

	case WEAPON_TYPE::PLASMA_AUTORIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z, CATEGORY::Guns);
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::PLASMA_SHOT, pos, dir);
		break;
	case WEAPON_TYPE::ENERGY_SHIELD:
		if (GetSound())
			GetSound()->playExternalSound(SOUNDS::soundEffectShield, pos.x, pos.y, pos.z, CATEGORY::Guns);
		break;

	case WEAPON_TYPE::DISC_GUN:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscGunStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscGun, pos.x, pos.y, pos.z, CATEGORY::Guns);
			}
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::DISC_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::MELEE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playMeleeStereo();
				}

				else
					GetSound()->playMelee(pos.x, pos.y, pos.z);
			}
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::MELEE_ATTACK, pos, dir);
		break;

	case WEAPON_TYPE::GRENADE_LAUNCHER:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncherStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncher, pos.x, pos.y, pos.z, CATEGORY::Guns);
			}
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::GRENADE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::SHOTGUN:
	{
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectShotGunStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectShotGun, pos.x, pos.y, pos.z, CATEGORY::Guns);
			}
		glm::vec3 rightV = normalize(cross(dir, vec3(0, 1, 0)));
		glm::vec3 upV = normalize(cross(dir, rightV));
		float xoff = 0.0f;
		float yoff = 0.0f;
		float r = 0.0f;
		glm::vec3 ndir;
		for (int k = 0; k < 10; k++)
		{
			xoff = glm::sin(k);
			yoff = glm::cos(k);
			r = ((rand() % 100) / 1500.0f) + 0.01f;
			rightV *= xoff*r;
			upV *= yoff*r;
			ndir = dir + upV + rightV;
			addBulletToList(conID, teamId, bulletId, BULLET_TYPE::SHOTGUN_PELLET, pos, ndir);
			rightV = normalize(cross(dir, vec3(0, 1, 0)));
			upV = normalize(cross(dir, rightV));
		}
	}
	break;

	case WEAPON_TYPE::FUSION_CANNON:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectLinkGunStereo, pos.x, pos.y, pos.z, CATEGORY::Guns);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectLinkGun, pos.x, pos.y, pos.z, CATEGORY::Guns);
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::FUSION_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::BATTERYWPN_SLOW:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playFieldsStereo();
				}
				else
					GetSound()->playFields(pos.x, pos.y, pos.z);

		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::BATTERY_SLOW_SHOT, pos, dir);

		break;

	case WEAPON_TYPE::BATTERYWPN_SPEED:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				if (conID == localPlayerId || conID == spectateID)
				{
					GetSound()->playFieldsStereo();
				}
				else
					GetSound()->playFields(pos.x, pos.y, pos.z);

		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::BATTERY_SPEED_SHOT, pos, dir);
		break;
	}
}

CONSUMABLE_TYPE Game::getConsumableUsed(int localPlayer)
{
	consumableUsed = false;
	Player* p = playerList[localPlayer];
	handleConsumableUse(localPlayer, p->getTeam(), itemUsed, p->getPos(), p->getDir());
	return itemUsed;
}

void Game::handleConsumableUse(int conID, int teamId, CONSUMABLE_TYPE ct, glm::vec3 pos, glm::vec3 dir)
{
	switch (ct)
	{
	case CONSUMABLE_TYPE::CLUSTERGRENADE:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::CLUSTER_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::OVERCHARGE:
		playerList[conID]->addModifier(OVERCHARGEMODIFIER);
		break;
	case CONSUMABLE_TYPE::VACUUMGRENADE:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::VACUUM_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::LIGHTSPEED:
		playerList[conID]->addModifier(LIGHTSPEEDMODIFIER);
		if (GetSound())
		{
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeedStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeed, pos.x, pos.y, pos.z, CATEGORY::Effects);
		}
		break;
	case CONSUMABLE_TYPE::THERMITEGRENADE:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::THERMITE_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::HACKINGDART:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::HACKING_DART, pos, dir);
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectHackingDart, pos.x, pos.y, pos.z, CATEGORY::Guns);
		break;
	}
}

SPECIAL_TYPE Game::getSpecialAbilityUsed(int localPlayer, int &sid)
{
	specialActivated = false;
	Player* p = playerList[localPlayer];
	sid = p->getRole()->getSpecialAbility()->getSpecialId();
	handleSpecialAbilityUse(localPlayer, p->getTeam(), sid, specialUsed, p->getPos(), p->getDir());
	return specialUsed;
}

void Game::handleSpecialAbilityUse(int conID, int teamId, int sID, SPECIAL_TYPE st, glm::vec3 pos, glm::vec3 dir)
{
	Player* p = playerList[conID];
	switch (st)
	{
	case SPECIAL_TYPE::LIGHTWALL:
	{
		if (!p->isLocal())
			p->setDir(dir);
		p->addModifier(MODIFIER_TYPE::LIGHTWALLCONTROLLOCK);
		addEffectToList(conID, teamId, sID, EFFECT_TYPE::LIGHT_WALL, pos, 0, 0.0f);

	}
	break;
	case SPECIAL_TYPE::THUNDERDOME:
		addEffectToList(conID, teamId, sID, EFFECT_TYPE::THUNDER_DOME, pos, 0, 0.0f);
		break;
	case SPECIAL_TYPE::MULTIJUMP:
	{
		if (GetSound())
		{
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectTrapperMultiJumpStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectTrapperMultiJump, pos.x, pos.y, pos.z, CATEGORY::Effects);
		}

		vec3 vel = p->getVelocity();
		if (vel.y < 0)
		{
			vel.y = 7.0f;
		}
		else
		{
			vel.y += 7.0f;
		}
		p->setGrounded(false);
		p->setVelocity(vel);
	}
	break;
	case SPECIAL_TYPE::WALLJUMP:
	{
		int size = 0;
		glm::vec4* cNorms = p->getCollisionNormalsForFrame(size);
		bool jumped = false;
		for (int c = 0; c < size && !jumped; c++)
		{
			if (cNorms[c].y < 0.5f && cNorms[c].y > -0.2f)
			{
				if (GetSound())
				{
					if (conID == localPlayerId || conID == spectateID)
					{
						GetSound()->playExternalSound(SOUNDS::soundEffectHunterJumpStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
					}

					else
						GetSound()->playExternalSound(SOUNDS::soundEffectHunterJump, pos.x, pos.y, pos.z, CATEGORY::Effects);
				}

				jumped = true;
				glm::vec3 reflect = normalize(glm::vec3(cNorms[c].x, 0, cNorms[c].z));
				glm::vec3 vel = p->getAirVelocity();
				vel.y = 0;
				vel = glm::reflect(vel, reflect);
				vel.y = 10.0f;
				vel.x *= 0.9f;
				vel.z *= 0.9f;
				p->setVelocity(vel);
			}
		}
	}
	break;
	case SPECIAL_TYPE::HACKINGDARTSPECIAL:
	{
		addBulletToList(conID, teamId, 0, BULLET_TYPE::HACKING_DART, pos, dir);
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectHackingDart, pos.x, pos.y, pos.z, CATEGORY::Guns);
	}
	break;
	case SPECIAL_TYPE::LIGHTSPEEDSPECIAL:
	{
		p->addModifier(MODIFIER_TYPE::LIGHTSPEEDMODIFIER);
		if (GetSound())
		{
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeedStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeed, pos.x, pos.y, pos.z, CATEGORY::Effects);
		}
	}
	break;
	case SPECIAL_TYPE::SPRINTD:		// D = Destroyer
	{
		p->addModifier(MODIFIER_TYPE::SPRINTCONTROLLOCK);
	}
	break;
	case SPECIAL_TYPE::DASH:
	{
		TrueGrit* temp = (TrueGrit*)p->searchModifierGet(MODIFIER_TYPE::TRUEGRITMODIFIER);
		if (temp)
		{
			temp->refresh();
		}
		else
			p->addModifier(MODIFIER_TYPE::TRUEGRITMODIFIER);
		if (GetSound())
		{
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectBruteDashStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectBruteDash, pos.x, pos.y, pos.z, CATEGORY::Effects);
		}
	}
	break;
	case SPECIAL_TYPE::GRAPPLINGHOOKSPECIAL:
	{
		addBulletToList(conID, teamId, 0, BULLET_TYPE::GRAPPLING_HOOK, pos, dir);

		if (GetSound())
		{
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectGrapplingShotStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectGrapplingShot, pos.x, pos.y, pos.z, CATEGORY::Effects);
		}
	}
	break;
	case SPECIAL_TYPE::CLEANSESPECIAL:
	{
		addEffectToList(conID, teamId, 0, EFFECT_TYPE::CLEANSENOVA, p->getPos(), 0, 0.0f);
		if (GetSound())
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectCleanseNovaStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}
			else
				GetSound()->playExternalSound(SOUNDS::soundEffectCleanseNova, pos.x, pos.y, pos.z, CATEGORY::Effects);
	}
	break;
	}
}

void Game::addEffectToList(int conID, int teamId, int effectId, EFFECT_TYPE et, glm::vec3 pos, int dmg, float interestingVariable)
{
	Effect* e = nullptr;
	Player* p = playerList[conID];

	int eid = -1, pid = -1;
	switch (et)
	{
	case EFFECT_TYPE::LIGHT_WALL:
		e = new LightwallEffect(p);
		if (GetSound())
			if (conID == localPlayerId || conID == spectateID)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectLightWallStereo, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectLightWall, pos.x, pos.y, pos.z, CATEGORY::Effects);
		break;
	case EFFECT_TYPE::THUNDER_DOME:
		e = new ThunderDomeEffect(p);
		break;
	case EFFECT_TYPE::EXPLOSION:
		e = new Explosion();
		e->setDamage(dmg);
		e->setInterestingVariable(interestingVariable);
		break;
	case EFFECT_TYPE::THERMITE_CLOUD:
		e = new ThermiteCloud();
		break;
	case EFFECT_TYPE::BATTERY_SLOW:
		teamId = 0;
		e = new BatteryFieldSlow();
		break;
	case EFFECT_TYPE::BATTERY_SPEED:
		teamId = 0;
		e = new BatteryFieldSpeed();
		break;
	case EFFECT_TYPE::VACUUM:
		e = new Vacuum();
		break;
	case EFFECT_TYPE::CLEANSENOVA:
		teamId = 0;
		e = new CleanseNova();
		break;
	case EFFECT_TYPE::HEALTHPACK:
		e = new HealthPack();
		break;
	case EFFECT_TYPE::HSCPICKUP:
		e = new HSCPickup();
		break;
	case EFFECT_TYPE::DOUBLEDAMAGEPICKUP:
		e = new DoubleDamagePickup();

	}
	e->init(conID, effectId, pos);
	e->setTeam(teamId);
	effects[et].push_back(e);
	addEffectToPhysics(e);

	if (gameState != SERVER)
	{
		EffectParticle ep;

		ep.pos = pos;
		ep.etype = et;
		if (teamId == 1)
		{
			if (et != EFFECT_TYPE::HEALTHPACK)
				ep.color = TEAMONECOLOR;
			else
				ep.color = TEAMTWOCOLOR;
		}
		else if (teamId == 2)
		{
			if (et != EFFECT_TYPE::HEALTHPACK)
				ep.color = TEAMTWOCOLOR;
			else
				ep.color = TEAMONECOLOR;
		}
		else
		{
			if (et == EFFECT_TYPE::BATTERY_SLOW)
				ep.color = SLOWBUBBLECOLOR;
			if (et == EFFECT_TYPE::BATTERY_SPEED)
				ep.color = SPEEDBUBBLECOLOR;
		}
		allEffectParticleSpawn.push_back(ep);
	}
}

void Game::addEffectToPhysics(Effect* effect)
{
	std::vector<float> eBox;
	int eid = -1, pid = -1;
	effect->getId(pid, eid);
	switch (effect->getType())
	{
	case EFFECT_TYPE::LIGHT_WALL:
		//Maybe add stuff here? not necessary imo
		break;
	default:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, effect->getType(), pid, eid);
		break;
	}
}

Effect* Game::getSpecificEffect(int PID, int SID, EFFECT_TYPE et, int &posInEffectArray)
{
	for (unsigned int c = 0; c < effects[et].size(); c++)
	{
		int p = -1;
		int e = -1;
		effects[et][c]->getId(p, e);
		if (p == PID && e == SID)
		{
			posInEffectArray = c;
			return effects[et][c];
		}
	}
	return nullptr;
}

Bullet* Game::getSpecificBullet(int PID, int BID, BULLET_TYPE bt, int &posInBulletArray)
{
	for (unsigned int c = 0; c < bullets[bt].size(); c++)
	{
		int p = -1;
		int b = -1;
		bullets[bt][c]->getId(p, b);
		if (p == PID && b == BID)
		{
			posInBulletArray = c;
			return bullets[bt][c];
		}
	}
	return nullptr;
}

int Game::handleBulletHitPlayerEvent(BulletHitPlayerInfo hi)
{
	Player* p = playerList[hi.playerHit];
	if (!p->didIDieThisFrame())
	{
		if (hi.bt == BULLET_TYPE::KILLYOURSELF)
		{
			p->setHP(0);
			p->addDeath();
			console->printMsg(p->getName() + suicideMessages[rand() % suicideMessages.size()], "System", 'S');
			return 0;
		}

		glm::vec3 pos = playerList[hi.playerHit]->getPos();
		if (gameState != Gamestate::SERVER)
		{
			if (GetSoundActivated())
			{
				if (hi.bt == BULLET_TYPE::HACKING_DART && p->isLocal())
				{
					GetSound()->playUserGeneratedSound(SOUNDS::hackedSound, CATEGORY::Effects);
				}
				else if (!p->isLocal())
					GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z, CATEGORY::Effects);
				else
					GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHitSelf, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}
		}
		int bulletPosInArray = -1;

		Bullet* theBullet = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, bulletPosInArray);
		if (playerList[hi.bulletPID] != nullptr && gameState == SERVER)
		{
			if (playerList[hi.bulletPID]->searchModifier(MODIFIER_TYPE::DOUBLEDAMAGEMOD))
			{
				p->setHP(p->getHP() - theBullet->getDamage());
			}
		}
		p->hitByBullet(theBullet, hi.bt, hi.newHPtotal);
		playerList[hi.bulletPID]->hitMarker = 0.25f;
		if (theBullet != nullptr && gameState != Gamestate::SERVER)
		{
			HitPosAndDirParticle hpad;
			hpad.pos = theBullet->getPos();
			//vec3 dir = -glm::normalize(theBullet->getDir());
			vec3 n = hpad.pos - p->getPos();
			//hpad.dir = reflect(dir, normalize(n));
			hpad.dir = hi.hitDir;
			if (p->getTeam() == 1)
				hpad.color = TEAMONECOLOR;
			else if (p->getTeam() == 2)
				hpad.color = TEAMTWOCOLOR;
			else hpad.color = vec3(1.0f, 1.0f, 1.0f);
			hpad.btype = hi.bt;
			allBulletHitPlayerPos.push_back(hpad);
		}
		if (p->getHP() == 0 && p->isAlive())
		{
			p->IdiedThisFrame();
			string fragMessage = fragMessages[rand() % fragMessages.size()];
			if (playerList[hi.bulletPID] != nullptr)
				console->printMsg(p->getName() + fragMessage + playerList[hi.bulletPID]->getName() + "!", "System", 'S');
			else
				console->printMsg(p->getName() + fragMessage + " a quitter!", "System", 'S');
			playerList[hi.bulletPID]->addKill();
			playerList[hi.bulletPID]->IncreaseFrags();
			playerList[hi.bulletPID]->ZeroDeaths();
			p->ZeroFrags();
			p->addDeath();
			p->IncreaseDeaths();
			if (playerList[hi.bulletPID]->GetConsecutiveFrags() == 3 && !playerList[hi.bulletPID]->killingSpreeDone)
			{
				console->printMsg(playerList[hi.bulletPID]->getName() + " is on a killing spree!", "System", 'S');
				if (GetSoundActivated() && hi.bulletPID == localPlayerId)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerKillingSpree, CATEGORY::Announcer);
				}
				playerList[hi.bulletPID]->killingSpreeDone = true;
			}

			else if (playerList[hi.bulletPID]->GetConsecutiveFrags() == 5 && !playerList[hi.bulletPID]->impressiveDone)
			{
				console->printMsg("Impressive, " + playerList[hi.bulletPID]->getName(), "System", 'S');
				if (GetSoundActivated() && hi.bulletPID == localPlayerId)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerImpressive, CATEGORY::Announcer);
				}
				playerList[hi.bulletPID]->impressiveDone = true;
			}
			addEffectToList(-1, p->getTeam(), hi.playerHit, EFFECT_TYPE::HEALTHPACK, p->getPos(), 0, 0.5f);
		}
		removeBullet(hi.bt, bulletPosInArray);
		int newHP = p->getHP();
		return newHP;
	}
	return 0;
}

int Game::handleEffectHitPlayerEvent(EffectHitPlayerInfo hi)
{
	Player* p = playerList[hi.playerHit];
	if (!p->didIDieThisFrame())
	{
		glm::vec3 pos = playerList[hi.playerHit]->getPos();
		if (gameState != Gamestate::SERVER)
		{
			if (GetSoundActivated())
			{
				if (hi.et == EFFECT_TYPE::HEALTHPACK)
					GetSound()->playExternalSound(SOUNDS::soundEffectHP, pos.x, pos.y, pos.z, CATEGORY::Effects);

				else if (hi.et != EFFECT_TYPE::BATTERY_SLOW && hi.et != EFFECT_TYPE::BATTERY_SPEED && hi.et != EFFECT_TYPE::HSCPICKUP && hi.et != EFFECT_TYPE::CLEANSENOVA && hi.et != EFFECT_TYPE::DOUBLEDAMAGEPICKUP)
				{
					if (!p->isLocal())
						GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z, CATEGORY::Effects);
					else
						GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHitSelf, pos.x, pos.y, pos.z, CATEGORY::Effects);
				}
			}
		}

		int effectPosInArray = -1;
		Effect* theEffect = getSpecificEffect(hi.effectPID, hi.effectID, hi.et, effectPosInArray);
		if (theEffect != nullptr)
		{
			theEffect->setPos(hi.hitPos);
			updateEffectBox(theEffect);
		}

		p->hitByEffect(theEffect, hi.et, hi.newHPtotal);

		switch (hi.et)
		{
		case EFFECT_TYPE::EXPLOSION:
		{
			if (gameState != Gamestate::SERVER)
			{
				vec3 normalFromExplosion = normalize(hi.playerPos - hi.hitPos);
				vec3 newVel = vec3(0);
				if (theEffect != nullptr)
					newVel = normalFromExplosion*theEffect->getInterestingVariable() / 3.0f;
				else
					newVel = normalFromExplosion / 3.0f; //If, by any chance, the effect got removed before this occured, we use a default explosion value of 1.0
				if (p->getGrounded())
				{
					p->setGrounded(false);
					if (theEffect != nullptr)
						newVel.y = theEffect->getInterestingVariable()*1.5f; //3.0f and 1.5f are arbitrary values
					else
						newVel.y = 1.5f;
				}

				p->setVelocity(newVel);
			}
		}
		break;
		case EFFECT_TYPE::VACUUM:
		{
			if (gameState != Gamestate::SERVER)
			{
				vec3 normalFromVacuum = normalize(hi.playerPos - hi.hitPos);
				vec3 newVel = vec3(0);
				if (theEffect != nullptr)
					newVel = -normalFromVacuum*theEffect->getInterestingVariable() / 6.0f;
				else
					newVel = -normalFromVacuum / 6.0f; //If, by any chance, the effect got removed before this occured, we use a default explosion value of 1.0
				if (p->getGrounded())
				{
					p->setGrounded(false);
					if (theEffect != nullptr)
						newVel.y = theEffect->getInterestingVariable()*0.5f;
					else
						newVel.y = 0.5f;
				}

				p->setVelocity(newVel);
			}
		}
		break;
		case EFFECT_TYPE::THERMITE_CLOUD:
			break;
		case EFFECT_TYPE::BATTERY_SLOW:
			if (p->searchModifier(MODIFIER_TYPE::BATTERYSLOWMOD))
			{
				BatterySlowMod* bsm = (BatterySlowMod*)(p->searchModifierGet(MODIFIER_TYPE::BATTERYSLOWMOD));
				bsm->refresh();
			}
			else
			{
				p->addModifier(MODIFIER_TYPE::BATTERYSLOWMOD);
			}
			break;
		case EFFECT_TYPE::BATTERY_SPEED:
			if (p->searchModifier(MODIFIER_TYPE::BATTERYSPEEDMOD))
			{
				BatterySpeedMod* bsm = (BatterySpeedMod*)(p->searchModifierGet(MODIFIER_TYPE::BATTERYSPEEDMOD));
				bsm->refresh();
			}
			else
			{
				p->addModifier(MODIFIER_TYPE::BATTERYSPEEDMOD);
			}
			break;
		case EFFECT_TYPE::CLEANSENOVA:
			p->cleanseModifiers();
			break;
		case EFFECT_TYPE::HEALTHPACK:
			if (gameState == SERVER)
				p->healing(25);
			if (theEffect != nullptr)
				removeEffect(EFFECT_TYPE::HEALTHPACK, effectPosInArray);
			break;
		case EFFECT_TYPE::HSCPICKUP:
		{
			HSCPickup* tester = (HSCPickup*)theEffect;
			if (gameState == SERVER)
				p->setHP(p->getMaxHP());
			p->getRole()->setSpecialMeter(100);
			p->getRole()->getConsumable()->reset();
			tester->startCooldown();

			if (gameState != SERVER && GetSound() && hi.playerHit == localPlayerId)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectHSCPickup, pos.x, pos.y, pos.z, CATEGORY::Effects);
			}
		}
		break;
		case EFFECT_TYPE::DOUBLEDAMAGEPICKUP:
		{
			DoubleDamagePickup* tester = (DoubleDamagePickup*)theEffect;
			tester->startCooldown();
			p->addModifier(MODIFIER_TYPE::DOUBLEDAMAGEMOD);

			if (GetSound() && p->isLocal())
			{
				GetSound()->playExternalSound(SOUNDS::announcerDoubleDamage, pos.x, pos.y, pos.z, CATEGORY::Announcer);
			}
		}
		break;
		default:
			break;
		}

		if (p->getHP() == 0 && p->isAlive())
		{
			p->IdiedThisFrame();
			string fragMessage = fragMessages[rand() % fragMessages.size()];
			if (playerList[hi.effectPID] != nullptr)
				console->printMsg(p->getName() + fragMessage + playerList[hi.effectPID]->getName() + "!", "System", 'S');
			else
				console->printMsg(p->getName() + fragMessage + " a quitter!", "System", 'S');
			playerList[hi.effectPID]->addKill();
			playerList[hi.effectPID]->IncreaseFrags();
			playerList[hi.effectPID]->ZeroDeaths();
			p->IncreaseDeaths();
			p->ZeroFrags();
			p->addDeath();

			if (playerList[hi.effectPID]->GetConsecutiveFrags() == 3 && !playerList[hi.effectPID]->killingSpreeDone)
			{
				console->printMsg(playerList[hi.effectPID]->getName() + " is on a killing spree!", "System", 'S');
				if (GetSoundActivated() && hi.effectPID == localPlayerId)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerKillingSpree, CATEGORY::Announcer);
				}
				playerList[hi.effectPID]->killingSpreeDone = true;
			}

			else if (playerList[hi.effectPID]->GetConsecutiveFrags() == 5 && !playerList[hi.effectPID]->impressiveDone)
			{
				console->printMsg("Impressive, " + playerList[hi.effectPID]->getName(), "System", 'S');
				if (GetSoundActivated() && hi.effectPID == localPlayerId)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerImpressive, CATEGORY::Announcer);
				}
				playerList[hi.effectPID]->impressiveDone = true;
			}
			addEffectToList(-1, p->getTeam(), hi.playerHit, EFFECT_TYPE::HEALTHPACK, p->getPos(), 0, 0.5f);

			/*if (p->GetConsecutiveDeaths() > 4 && !p->roleChangeWritten)
			{
				console->printMsg("You can now change role if you want to!", "System", 'S');
				p->roleChangeWritten = true;
			}*/
		}



		int newHP = p->getHP();
		return newHP;
	}
	return 0;
}

void Game::bounceBullet(BulletHitWorldInfo hwi, Bullet* theBullet)
{
	theBullet->bounce();

	vec4 combinedNormal(0); // all normals added together and then normalized
	vec3 posadjust(0); //pendepths combined

	posadjust += normalize(vec3(hwi.collisionNormal)) * hwi.collisionNormal.w;
	combinedNormal += hwi.collisionNormal;

	// the w component holds the pendepth
	vec3 combinedNormal2 = vec3(combinedNormal);

	//reflect!!
	if (combinedNormal2 != glm::vec3(0, 0, 0))
	{
		combinedNormal2 = normalize(combinedNormal2);

		vec3 dir = normalize(reflect(theBullet->getDir(), combinedNormal2));

		//vec3 dirn = vec3(theBullet->getDir());
		//dirn = normalize(dirn);

		float d = dot(dir, combinedNormal2);

		if (d < 0.3f && d >= 0.0f)
			dir = normalize((dir * (1 - d)) + (combinedNormal2 * d));
		/*
		//float dotp = dot(dirn, obb->planes[n].n);
		//if (dotp < 0.9f && dotp > -0.9f)
		//{
			//vec3 right = obb->planes[n].v1; //goes to the right
			//vec3 left = -right;
			//rotate it to an angle to  increase the dot product. so TOWARDS the normal face
			//dirn = dirn * 0.9f + obb->planes[n].n * 0.1f;
			dirn = normalize(dirn);
			if (dot(dir, right) > 0.0f)//if we're moving to the right
			{
				//We need to angle one direction

			}
			else if (dot(dir, left) > 0.0f)//We're moving ot the left
			{
				//We need to angle opposite direction
			}
			else
			{

			}
			t.x = dir.x;
			t.y = dir.y;
			t.z = dir.z;
		}*/


		theBullet->setDir(dir);

		//use pendepth to set a new pos 
		if (theBullet->getType() == BULLET_TYPE::DISC_SHOT)
			theBullet->setPos(theBullet->getPos() + vec3(posadjust));
		//theBullet->setPos(theBullet->getPos() + (theBullet->getDir() * theBullet->getVel() * lastDT * 1.0f));

		// remove bullet code
		//BulletHitWorldInfo hi;
		//hi.bt = BULLET_TYPE(b); hi.hitPos = bullets[b][j]->getPos(); hi.hitDir = bullets[b][j]->getDir();
		//bullets[b][j]->getId(hi.bulletPID, hi.bulletBID);
		//hi.collisionNormal = collides;
		//allBulletHitsOnWorld.push_back(hi);
		//handleBulletHitWorldEvent(hi);
	}
}

void Game::handleBulletHitWorldEvent(BulletHitWorldInfo hi)
{
	//hit info and bullet rad (assumed 0.6 for all)
	decalAdd(hi, 0.2f);

	int arraypos = -1;
	Bullet* b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, arraypos);

	if (b != nullptr)
	{
		if (gameState != Gamestate::SERVER)
		{
			HitPosAndDirParticle hpad;
			hpad.pos = hi.hitPos - vec3(hi.collisionNormal) * (0.35f - hi.collisionNormal.w);
			hpad.dir = reflect(hi.hitDir, vec3(hi.collisionNormal));
			if (b->getTeam() == 1)
				hpad.color = TEAMONECOLOR;
			else if (b->getTeam() == 2)
				hpad.color = TEAMTWOCOLOR;
			else
				hpad.color = vec3(1, 1, 1);

			if (b->getType() != BULLET_TYPE::GRAPPLING_HOOK)
				hpad.btype = BULLET_TYPE::NROFBULLETS;
			else
				hpad.btype = BULLET_TYPE::GRAPPLING_HOOK;

			allBulletHitPlayerPos.push_back(hpad);
		}

		if (b->getType() != BULLET_TYPE::DISC_SHOT)
			b->setPos(hi.hitPos + (vec3(normalize(hi.collisionNormal)) * hi.collisionNormal.w));
		else
			b->setPos(hi.hitPos);
		b->setDir(hi.hitDir);
		vec3 temp;
		vec3 vel = b->getVel();

		switch (hi.bt)
		{
		case BULLET_TYPE::CLUSTER_GRENADE:
			if (b->getBounces() > 3)
			{
				removeBullet(hi.bt, arraypos);
				break;
			}
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
			vel *= 0.7f;
			b->setVel(vel);
			bounceBullet(hi, b);
			temp = b->getDir();
			temp.x *= 0.8f;
			temp.y *= 0.6f;
			temp.z *= 0.8f;
			b->setDir(temp);
			break;
		case BULLET_TYPE::CLUSTERLING:
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
			vel *= 0.7f;
			b->setVel(vel);
			bounceBullet(hi, b);
			temp = b->getDir();
			temp.x *= 0.8f;
			temp.y *= 0.6f;
			temp.z *= 0.8f;
			b->setDir(temp);
			break;
		case BULLET_TYPE::THERMITE_GRENADE:
			if (b->getBounces() > 1)
			{
				removeBullet(hi.bt, arraypos);
				break;
			}
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
			vel *= 0.7f;
			b->setVel(vel);
			bounceBullet(hi, b);
			temp = b->getDir();
			temp.x *= 0.8f;
			temp.y *= 0.6f;
			temp.z *= 0.8f;
			b->setDir(temp);
			break;
		case BULLET_TYPE::VACUUM_GRENADE:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::DISC_SHOT:
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectDiscBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
			bounceBullet(hi, b);
			break;
		case BULLET_TYPE::GRENADE_SHOT:
			if (b->getBounces() > 3)
			{
				removeBullet(hi.bt, arraypos);
				break;
			}
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncherBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
			vel *= 0.7f;
			b->setVel(vel);
			bounceBullet(hi, b);
			temp = b->getDir();
			temp.x *= 0.8f;
			temp.y *= 0.6f;
			temp.z *= 0.8f;

			b->setDir(temp);
			break;
		case BULLET_TYPE::GRAPPLING_HOOK:
			temp = normalize(b->getPos() - playerList[hi.bulletPID]->getPos());
			temp *= 2.5f;
			temp.y += temp.y*3.5f + 5.0f;
			playerList[hi.bulletPID]->setVelocity(temp);
			playerList[hi.bulletPID]->setGrounded(false);

			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrapplingHook, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);

			removeBullet(hi.bt, arraypos);
			break;
		default:
			removeBullet(hi.bt, arraypos);
			break;
		}

	}
}

void Game::handleBulletHitEffectEvent(BulletHitEffectInfo hi)
{
	int arraypos = -1;
	Bullet* b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, arraypos);
	if (hi.bt == BULLET_TYPE::BATTERY_SLOW_SHOT || hi.bt == BULLET_TYPE::BATTERY_SPEED_SHOT || hi.bt == BULLET_TYPE::FUSION_SHOT) //Battery Field exceptions
	{
		if (hi.et == EFFECT_TYPE::BATTERY_SLOW || hi.et == EFFECT_TYPE::BATTERY_SPEED)
		{
			int apos = -1;
			Effect* e = getSpecificEffect(hi.effectPID, hi.effectID, hi.et, apos);
			if (e != nullptr)
			{
				if (hi.bt == (BULLET_TYPE::FUSION_SHOT))
				{
					addEffectToList(hi.effectPID, hi.bulletTeam, hi.effectID, EFFECT_TYPE::EXPLOSION, e->getPos(), 30, 8.0f);
					if (GetSound())
					{
						GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
					}
				}

				else
				{
					addEffectToList(hi.effectPID, hi.bulletTeam, hi.effectID, EFFECT_TYPE::EXPLOSION, e->getPos(), 25, 5.0f);
					if (GetSound())
					{
						GetSound()->playExternalSound(SOUNDS::soundEffectClusterlingExplosion, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z, CATEGORY::Guns);
					}
				}

				removeEffect(hi.et, apos);
			}
			if (b != nullptr)
				b->setSpawnAdditionals(false);
		}
	}
	if (b != nullptr)
	{
		if (hi.et != EFFECT_TYPE::BATTERY_SLOW && hi.et != EFFECT_TYPE::BATTERY_SPEED)
		{
			removeBullet(hi.bt, arraypos);
			HitPosAndDirParticle hpad;
			hpad.pos = hi.hitPos;
			//vec3 dir = -glm::normalize(theBullet->getDir());
			vec3 n = (hi.hitPos + hi.hitDir) - hpad.pos;
			//hpad.dir = reflect(dir, normalize(n));
			hpad.dir = hi.hitDir;
			if (hi.bulletTeam == 1)
				hpad.color = TEAMONECOLOR;
			else if (hi.bulletTeam == 2)
				hpad.color = TEAMTWOCOLOR;
			else hpad.color = vec3(1.0f, 1.0f, 1.0f);
			hpad.btype = BULLET_TYPE::EFFECTIMPACT;
			allBulletHitPlayerPos.push_back(hpad);
		}
	}
}

void Game::handleEffectHitEffectEvent(EffectHitEffectInfo hi)
{
	/*int arraypos = -1;
	Effect* e = getSpecificEffect(hi.effectPID, hi.effectSID, hi.et, hi.posInArr);

	if (e != nullptr)
	{
		switch (hi.et)
		{
		case EFFECT_TYPE::EXPLOSION:
			removeEffect(hi.et, hi.posInArr);
			break;
		case EFFECT_TYPE::LIGHT_WALL:
			removeEffect(hi.et, hi.posInArr);
			break;
		case EFFECT_TYPE::CLEANSEEXPLOSION:
			removeEffect(hi.et, hi.posInArr);
			break;
		}
	}*/
}

void Game::handleBulletTimeOuts(BulletTimeOutInfo hi)
{
	Bullet* b;
	int posInArray = -1;
	b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, posInArray);
	if (b != nullptr)
	{
		b->setPos(hi.pos);
		if (hi.bt == BULLET_TYPE::BATTERY_SLOW_SHOT || hi.bt == BULLET_TYPE::BATTERY_SPEED_SHOT)
			b->setSpawnAdditionals(false);
		removeBullet(hi.bt, posInArray);
	}
}

void Game::handleEffectTimeOuts(EffectTimeOutInfo hi)
{
	Effect* e;
	int posInArray = -1;
	e = getSpecificEffect(hi.effectPID, hi.effectID, hi.et, posInArray);
	if (e != nullptr)
	{
		e->setPos(hi.pos);
		removeEffect(hi.et, posInArray);
	}

}

void Game::removeEffect(EFFECT_TYPE et, int posInArray)
{
	int pid = -1, eid = -1;
	effects[et][posInArray]->getId(pid, eid);
	physics->removeEffect(eid, pid, et);
	delete effects[et][posInArray];
	effects[et][posInArray] = effects[et][effects[et].size() - 1];
	effects[et].pop_back();
}

void Game::removeBullet(BULLET_TYPE bt, int posInArray)
{
	int PID = 0, BID = 0;

	if (posInArray <= bullets[bt].size())
	{
		Bullet* parent = bullets[bt][posInArray];
		if (parent != nullptr)
		{
			parent->getId(PID, BID);
			switch (bt)
			{
			case BULLET_TYPE::CLUSTER_GRENADE: //FUCKING EVERYTHING	
			{
				vec3 lingDir;
				lingDir = parent->getDir();
				lingDir.x += 0.35f;
				lingDir.z += 0.35f;
				addBulletToList(PID, parent->getTeam(), BID, CLUSTERLING, parent->getPos(), lingDir);
				lingDir.x = -lingDir.x;
				addBulletToList(PID, parent->getTeam(), BID + 1, CLUSTERLING, parent->getPos(), lingDir);
				lingDir.z = -lingDir.z;
				addBulletToList(PID, parent->getTeam(), BID + 2, CLUSTERLING, parent->getPos(), lingDir);
				lingDir.x = -lingDir.x;
				addBulletToList(PID, parent->getTeam(), BID + 3, CLUSTERLING, parent->getPos(), lingDir);

				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 35, 6.0f);

				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);
				break;
			}
			case BULLET_TYPE::CLUSTERLING:
			{
				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 10, 3.0f);

				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectClusterlingExplosion, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);
				break;
			}
			case BULLET_TYPE::CLEANSE_BOMB:
			{
				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 0, 4.0f);
				break;
			}
			case BULLET_TYPE::VACUUM_GRENADE:
			{
				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::VACUUM, parent->getPos(), 10, 4.0f);

				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectVacuumGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);

				break;
			}
			case BULLET_TYPE::THERMITE_GRENADE:
			{
				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::THERMITE_CLOUD, parent->getPos(), 10, 0.0f);
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectThermiteGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);
				break;
			}
			case BULLET_TYPE::GRENADE_SHOT:
			{
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);

				addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 20, 4.0f);
				break;
			}
			case BULLET_TYPE::BATTERY_SLOW_SHOT:
				if (parent->getSpawnAdditionals())
				{
					addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::BATTERY_SLOW, parent->getPos(), 0, 0.0f);
					if (GetSoundActivated())
						GetSound()->playExternalSound(SOUNDS::soundEffectBatteryFields, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);
				}

				break;
			case BULLET_TYPE::BATTERY_SPEED_SHOT:
				if (parent->getSpawnAdditionals())
				{
					addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::BATTERY_SPEED, parent->getPos(), 0, 0.0f);
					if (GetSoundActivated())
						GetSound()->playExternalSound(SOUNDS::soundEffectBatteryFields, parent->getPos().x, parent->getPos().y, parent->getPos().z, CATEGORY::Guns);
				}
				break;
			}
			delete bullets[bt][posInArray];
			bullets[bt][posInArray] = bullets[bt][bullets[bt].size() - 1];
			bullets[bt].pop_back();
		}
	}
}

bool Game::playerWantsToRespawn()
{
	if (localPlayerRespawnWaiting)
		return false;
	else if (localPlayerWantsRespawn)
		localPlayerRespawnWaiting = true;
	return localPlayerWantsRespawn;
}

void Game::allowPlayerRespawn(int p_conID, int respawnPosition)
{
	if (playerList[p_conID] != nullptr)
	{
		playerList[p_conID]->respawn(spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].pos, spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].dir, spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].roomID);
		if (p_conID == localPlayerId)
		{
			localPlayerWantsRespawn = false;
			localPlayerRespawnWaiting = false;
			freecam = false;
			spectateID = -1;
		}
	}
}

void Game::denyPlayerRespawn(char tryAgain)
{
	if (tryAgain == 'Y')
	{
		localPlayerRespawnWaiting = false;
	}
}

int Game::findPlayerPosInTeam(int conID)
{
	for (int i = 0; i < teamOne.size(); i++)
	{
		if (conID == teamOne[i])
		{
			return i;
		}
	}
	for (int i = 0; i < teamTwo.size(); i++)
	{
		if (conID == teamTwo[i])
		{
			return i;
		}
	}

	return 0;
}

bool Game::checkIfPlayerCanRespawn(int conid, char &tryAgain)
{
	if (gamemode->getType() == GAMEMODE_TYPE::KOTH)
	{
		KingOfTheHill* koth = (KingOfTheHill*)gamemode;
		bool canRespawn = koth->playerRespawn(conid);
		if (canRespawn)
			return true;
		else
		{
			if (koth->getRespawnTokens(playerList[conid]->getTeam()) > 0)
			{
				tryAgain = 'Y';
			}
			return false;
		}
	}
	return false;
}

int Game::GetGameState()
{
	return this->gameState;
}

int Game::GetLocalPlayerId()
{
	return this->localPlayerId;
}

unsigned int Game::getNrOfDecals()
{
	return decalCounter;
}

Decal_GameInfo* Game::getAllDecalGameInfo()
{
	return &decals_gameInfo[0];
}

Decal_RenderInfo* Game::getAllDecalRenderInfo()
{
	return &decals_renderInfo[0];
}

void Game::updateDecals(float dt)
{
	float inten = 0;

	for (unsigned int n = 0; n < decalCounter; n++)
	{
		if (decals_gameInfo[n].update_true_if_dead(dt, inten))
		{
			//if timeout
			decals_gameInfo[n] = decals_gameInfo[decalCounter - 1];
			decals_renderInfo[n] = decals_renderInfo[decalCounter - 1];
			decalCounter--;
			n--;
		}
		else // still alive, set new inten
			decals_renderInfo[n].inten = inten;
	}
}

void Game::decalAdd(BulletHitWorldInfo info, float rad)
{
	if (decalCounter >= Max_Decals)
		return;

	Player* p = getPlayer(info.bulletPID);
	if (p == nullptr)
		return;

	decals_gameInfo[decalCounter].lifeLeft = decals_gameInfo[decalCounter].lifeTime = 5;

	decals_renderInfo[decalCounter].inten = 1.0f;
	decals_renderInfo[decalCounter].normal = vec3(info.collisionNormal);
	//for correct; pos = pos - collision.normal * (Bulletrad - pendepth)
	decals_renderInfo[decalCounter].pos = info.hitPos - vec3(info.hitDir) * (info.collisionNormal.w);

	if (p->getTeam() == 1)
		decals_renderInfo[decalCounter].color = TEAMONECOLOR;
	else
		decals_renderInfo[decalCounter].color = TEAMTWOCOLOR;

	decalCounter++;
}

void Game::cullingPointvsRoom(glm::vec3* pos, int* arr_interIDs, int& interCount, int maxsize)
{
	physics->cullingPointvsRoom(pos, arr_interIDs, interCount, maxsize);
}

void Game::clearAllPlayerKD()
{
	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
			playerList[c]->clearKD();
	}
}

void Game::resetAllPickups()
{
	int size = effects[EFFECT_TYPE::HSCPICKUP].size();
	HSCPickup* temp;
	for (int i = 0; i < size; i++)
	{
		temp = (HSCPickup*)effects[EFFECT_TYPE::HSCPICKUP][i];
		temp->startCooldown();

	}
	int size2 = effects[EFFECT_TYPE::DOUBLEDAMAGEPICKUP].size();
	DoubleDamagePickup* temp2;
	for (int i = 0; i < size2; i++)
	{
		temp2 = (DoubleDamagePickup*)effects[EFFECT_TYPE::DOUBLEDAMAGEPICKUP][i];
		temp2->startCooldown();
	}
}

void Game::fixLocalPlayerCamera(float dt)
{
	bool spectatingThis = false;
	bool spectating = false;

	if (spectateID > -1)
	{
		spectating = true;
		playerList[spectateID]->fixCamera(dt, freecam, spectating);
	}
	else if (!playerList[localPlayerId]->getLockedControls())
		playerList[localPlayerId]->fixCamera(dt, freecam, spectating);
}