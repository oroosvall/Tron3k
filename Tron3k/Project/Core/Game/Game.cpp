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
	if (GetSoundActivated() && musicVolumeForMenu > 0 && GetSound()->getFading())
	{
		musicVolumeForMenu--;
		GetSound()->setVolumeMusic(musicVolumeForMenu);
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
		checkPlayerVWorldCollision(dt);
		checkBulletVWorldCollision(dt);
		checkPlayerVEffectCollision();
		checkBulletVEffectCollision(dt);
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
		checkBulletVWorldCollision(dt);
		checkPlayerVBulletCollision();
		checkPlayerVEffectCollision();
		checkBulletVEffectCollision(dt);
		checkPlayerVCapCollision();
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
			float height = 2.5f;

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
	case EFFECT_TYPE::CLEANSEEXPLOSION:

		break;
	case EFFECT_TYPE::THERMITE_CLOUD:

		break;
	case EFFECT_TYPE::VACUUM:

		break;
	case EFFECT_TYPE::ZEROFRICTION:

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

			if (playerList[i]->getFootsteps() && (playerList[i]->getAnimState_t_p() != AnimationState::third_primary_air && playerList[i]->getAnimState_t_p() != AnimationState::first_primary_air))
			{
				glm::vec3 pos;
				glm::vec3 vel;
				pos = playerList[i]->getPos();
				vel = playerList[i]->getVelocity();
				if (vel.x != 0 || vel.z != 0)
				{
					playerList[i]->setFootstepsCountdown();
					playerList[i]->setFootstepsLoop(false);
					if (GetSoundActivated())
						GetSound()->playFootsteps(playerList[i]->getRole()->getRole(), pos.x, pos.y, pos.z);
				}

			}

			if (!playerList[i]->CheckAbleToJumpSound())
			{
				playerList[i]->CoolDownJump(dt);
			}

			if (!playerList[i]->CheckAbleToJumpSound() && (playerList[i]->getAnimState_t_p() == AnimationState::third_primary_air || playerList[i]->getAnimState_t_p() == AnimationState::first_primary_air))
			{
				playerList[i]->jumpSoundAble = true;
			}

			if (playerList[i]->CheckAbleToJumpSound() && (playerList[i]->getAnimState_t_p() != AnimationState::third_primary_air && playerList[i]->getAnimState_t_p() != AnimationState::first_primary_air))
			{
				glm::vec3 pos;
				pos = playerList[i]->getPos();
				playerList[i]->SetJumpCoolDown(100.0f);

				if (GetSoundActivated())
					GetSound()->playJump(playerList[i]->getRole()->getRole(), pos.x, pos.y, pos.z);


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
				collNormalWalls = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::LIGHT_WALL, eid);
				if (collNormalWalls != vec4(0, 0, 0, 0))
					playerList[localPlayerId]->addEffectCollisionNormal(collNormalWalls);
			}

		}
		size = (int)effects[EFFECT_TYPE::THUNDER_DOME].size();
		for (int c = 0; c < size; c++)
		{
			int eid = -1, pid = -1;
			effects[EFFECT_TYPE::THUNDER_DOME][c]->getId(pid, eid);
			collNormalDomes = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::THUNDER_DOME, eid);
			if (collNormalDomes != vec4(0, 0, 0, 0))
				playerList[localPlayerId]->addEffectCollisionNormal(collNormalDomes);
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
							if (pid != j && effects[t][i]->getTeam() != playerList[j]->getTeam())
							{
								collNormals = physics->checkPlayerVEffectCollision(playerList[j]->getPos(), t, eid);
								if (collNormals != vec4(0, 0, 0, 0))
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

void Game::checkPlayerVBulletCollision()
{
	glm::vec3 collides = glm::vec3(0, 0, 0);

	for (int i = 0; i < max_con; i++)
	{
		Player* p = playerList[i];
		if (p != nullptr)
		{
			for (unsigned int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
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
								collides = physics->checkPlayerVBulletCollision(playerList[i]->getPos() - (vec3(0, playerList[i]->getRole()->getBoxModifier(), 0)), bullets[b][j]->getPos(), this->playerList[i]->getRole()->GetSize());
						}
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
							allBulletHitsOnPlayers.push_back(hit);
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
						bi.et = EFFECT_TYPE::THUNDER_DOME;
						bi.hitPos = bPos;
						bi.hitDir = bullets[b][j]->getDir();
						bi.collisionNormal = collNormalDomes;
						allBulletHitsOnEffects.push_back(bi);
					}
					//glm::vec3 vel = bullets[b][j]->getPos()->getVelocity();
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
		b = new BatterySlowShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::BATTERY_SPEED_SHOT:
		b = new BatterySpeedShot(pos, dir, conID, bulletId, teamId);
		break;
	case BULLET_TYPE::LINK_SHOT:
		b = new LinkShot(pos, dir, conID, bulletId, teamId);
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
		b = new HackingDart(pos, dir, conID, bulletId, teamId, BULLET_TYPE::HACKING_DART);
		break;
	case BULLET_TYPE::MELEE_ATTACK:
		b = new MeleeAttack(vec3(999.0f, 999.0f, 999.0f), dir, conID, bulletId, teamId, p);
		break;
	}

	bullets[bt].push_back(b);
}

void Game::handleWeaponFire(int conID, int teamId, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir)
{

	switch (weapontype)
	{
	case WEAPON_TYPE::PULSE_RIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectPulseRifleShotStereo, pos.x, pos.y, pos.z);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z);
			}

		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::PULSE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::ENERGY_BOOST:
		if (GetSoundActivated())
			if (GetSound())
			{
				if (conID == localPlayerId)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectEnergyBoostStereo, pos.x, pos.y, pos.z);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectEnergyBoost, pos.x, pos.y, pos.z);
			}
		playerList[conID]->healing(15);
		playerList[conID]->getRole()->swapWeapon(WEAPON_TYPE::ENERGY_BOOST, 0);
		break;

	case WEAPON_TYPE::PLASMA_AUTORIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z);
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::PLASMA_SHOT, pos, dir);
		break;
	case WEAPON_TYPE::ENERGY_SHIELD:
		if (GetSound())
			GetSound()->playExternalSound(SOUNDS::soundEffectShield, pos.x, pos.y, pos.z);
		break;

	case WEAPON_TYPE::DISC_GUN:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscGunStereo, pos.x, pos.y, pos.z);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscGun, pos.x, pos.y, pos.z);
			}
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::DISC_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::MELEE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId)
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
				if (conID == localPlayerId)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncherStereo, pos.x, pos.y, pos.z);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncher, pos.x, pos.y, pos.z);
			}
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::GRENADE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::SHOTGUN:
	{
		if (gameState != Gamestate::SERVER)
			if (GetSound())
			{
				if (conID == localPlayerId)
				{
					GetSound()->playExternalSound(SOUNDS::soundEffectShotGunStereo, pos.x, pos.y, pos.z);
				}

				else
					GetSound()->playExternalSound(SOUNDS::soundEffectShotGun, pos.x, pos.y, pos.z);
			}
		glm::vec3 rightV = normalize(cross(dir, vec3(0, 1, 0)));
		glm::vec3 upV = normalize(cross(dir, rightV));
		for (int k = 0; k < 10; k++)
		{
			float xoff = glm::sin(k);
			float yoff = glm::cos(k);
			float r = ((rand() % 100) / 2000.0f) + 0.01f;
			rightV *= xoff*r;
			upV *= yoff*r;
			glm::vec3 ndir = dir + upV + rightV;
			addBulletToList(conID, teamId, bulletId, BULLET_TYPE::SHOTGUN_PELLET, pos, ndir);
			rightV = normalize(cross(dir, vec3(0, 1, 0)));
			upV = normalize(cross(dir, rightV));
		}
	}
	break;

	case WEAPON_TYPE::LINK_GUN:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectLinkGun, pos.x, pos.y, pos.z);
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::LINK_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::BATTERYFIELD_SLOW:
		addBulletToList(conID, teamId, bulletId, BULLET_TYPE::BATTERY_SLOW_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::BATTERYFIELD_SPEED:
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
			if (conID == localPlayerId)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeedStereo, pos.x, pos.y, pos.z);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeed, pos.x, pos.y, pos.z);
		}
		break;
	case CONSUMABLE_TYPE::THERMITEGRENADE:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::THERMITE_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::HACKINGDART:
		addBulletToList(conID, teamId, 0, BULLET_TYPE::HACKING_DART, pos, dir);
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectHackingDart, pos.x, pos.y, pos.z);
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
		int arraypos = -1;
		//Effect* lwe = getSpecificEffect(conID, sID - 1, EFFECT_TYPE::LIGHT_WALL, arraypos);
		addEffectToList(conID, teamId, sID, EFFECT_TYPE::LIGHT_WALL, pos, 0, 0.0f);

	}
	break;
	case SPECIAL_TYPE::THUNDERDOME:
		if (!p->isLocal())
			p->setDir(dir);
		addEffectToList(conID, teamId, sID, EFFECT_TYPE::THUNDER_DOME, pos, 0, 0.0f);
		break;
	case SPECIAL_TYPE::MULTIJUMP:
	{
		if (GetSound())
		{
			if (conID == localPlayerId)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectTrapperMultiJumpStereo, pos.x, pos.y, pos.z);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectTrapperMultiJump, pos.x, pos.y, pos.z);
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
					if (conID == localPlayerId)
					{
						GetSound()->playExternalSound(SOUNDS::soundEffectHunterJumpStereo, pos.x, pos.y, pos.z);
					}

					else
						GetSound()->playExternalSound(SOUNDS::soundEffectHunterJump, pos.x, pos.y, pos.z);
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
			GetSound()->playExternalSound(SOUNDS::soundEffectHackingDart, pos.x, pos.y, pos.z);
	}
	break;
	case SPECIAL_TYPE::LIGHTSPEEDSPECIAL:
	{
		p->addModifier(MODIFIER_TYPE::LIGHTSPEEDMODIFIER);
		if (GetSound())
		{
			if (conID == localPlayerId)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeedStereo, pos.x, pos.y, pos.z);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeed, pos.x, pos.y, pos.z);
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
		p->addModifier(MODIFIER_TYPE::TRUEGRITMODIFIER);
		if (GetSound())
		{
			if (conID == localPlayerId)
			{
				GetSound()->playExternalSound(SOUNDS::soundEffectBruteDashStereo, pos.x, pos.y, pos.z);
			}

			else
				GetSound()->playExternalSound(SOUNDS::soundEffectBruteDash, pos.x, pos.y, pos.z);
		}
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
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectLightWall, pos.x, pos.y, pos.z);
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
	case EFFECT_TYPE::HEALTHPACK:
		e = new HealthPack();
		break;
	}
	e->init(conID, effectId, pos);
	e->setTeam(teamId);
	effects[et].push_back(e);
	addEffectToPhysics(e);
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
	case EFFECT_TYPE::THUNDER_DOME:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, EFFECT_TYPE::THUNDER_DOME, pid, eid);
		break;
	case EFFECT_TYPE::EXPLOSION:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, EFFECT_TYPE::EXPLOSION, pid, eid);
		break;
	case EFFECT_TYPE::THERMITE_CLOUD:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, EFFECT_TYPE::THERMITE_CLOUD, pid, eid);
		break;
	case EFFECT_TYPE::HEALTHPACK:
		eBox.push_back(effect->getPos().x);
		eBox.push_back(effect->getPos().y);
		eBox.push_back(effect->getPos().z);
		eBox.push_back(effect->getInterestingVariable());
		physics->receiveEffectBox(eBox, EFFECT_TYPE::HEALTHPACK, pid, eid);
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
			console->printMsg(p->getName() + " gave up on life.", "System", 'S');
			return 0;
		}
		if (hi.bt != BULLET_TYPE::CLUSTERLING)	//Any bullets that should not detonate on contact
		{
			glm::vec3 pos = playerList[hi.playerHit]->getPos();
			if (gameState != Gamestate::SERVER)
			{
				if (GetSoundActivated())
				{
					if (hi.bt == BULLET_TYPE::HACKING_DART && p->isLocal())
					{
						GetSound()->playUserGeneratedSound(SOUNDS::hackedSound);
					}
					else if (!p->isLocal())
						GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z);
					else
						GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHitSelf, pos.x, pos.y, pos.z);
				}
			}
			int bulletPosInArray = -1;

			Bullet* theBullet = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, bulletPosInArray);

			p->hitByBullet(theBullet, hi.bt, hi.newHPtotal);
			if (p->getHP() == 0 && p->isAlive())
			{
				p->IdiedThisFrame();
				if (playerList[hi.bulletPID] != nullptr)
					console->printMsg(p->getName() + " was fragged by " + playerList[hi.bulletPID]->getName() + "!", "System", 'S');
				else
					console->printMsg(p->getName() + " was fragged by a quitter!", "System", 'S');
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
						GetSound()->playUserGeneratedSound(SOUNDS::announcerKillingSpree);
					}
					playerList[hi.bulletPID]->killingSpreeDone = true;
				}

				else if (playerList[hi.bulletPID]->GetConsecutiveFrags() == 5 && !playerList[hi.bulletPID]->impressiveDone)
				{
					console->printMsg("Impressive, " + playerList[hi.bulletPID]->getName(), "System", 'S');
					if (GetSoundActivated() && hi.bulletPID == localPlayerId)
					{
						GetSound()->playUserGeneratedSound(SOUNDS::announcerImpressive);
					}
					playerList[hi.bulletPID]->impressiveDone = true;
				}
				glm::vec3 hpPos = p->getPos();
				hpPos.y += 0.5f;
				addEffectToList(-1, p->getTeam(), hi.playerHit, EFFECT_TYPE::HEALTHPACK, hpPos, 0, 0.5f);
			}
			removeBullet(hi.bt, bulletPosInArray);
		}
		else
			return p->getHP();
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
					GetSound()->playExternalSound(SOUNDS::soundEffectHP, pos.x, pos.y, pos.z);
				else if (!p->isLocal())
					GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z);
				else
					GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHitSelf, pos.x, pos.y, pos.z);
			}
		}

		int effectPosInArray = -1;
		Effect* theEffect = getSpecificEffect(hi.effectPID, hi.effectID, hi.et, effectPosInArray);
		if (theEffect != nullptr)
		{
			theEffect->setPos(hi.hitPos);
			updateEffectBox(theEffect);
		}

		p->hitByEffect(theEffect, hi.newHPtotal);

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
		case EFFECT_TYPE::THERMITE_CLOUD:
			break;
		case EFFECT_TYPE::BATTERY_SLOW:
			break;
		case EFFECT_TYPE::HEALTHPACK:
			if (gameState == SERVER)
				p->healing(25);
			if (theEffect != nullptr)
				removeEffect(EFFECT_TYPE::HEALTHPACK, effectPosInArray);
			break;
		default:
			break;
		}

		if (p->getHP() == 0 && p->isAlive())
		{
			p->IdiedThisFrame();
			if (playerList[hi.effectPID] != nullptr)
				console->printMsg(p->getName() + " was fragged by " + playerList[hi.effectPID]->getName() + "!", "System", 'S');
			else
				console->printMsg(p->getName() + " was fragged by a quitter!", "System", 'S');
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
					GetSound()->playUserGeneratedSound(SOUNDS::announcerKillingSpree);
				}
				playerList[hi.effectPID]->killingSpreeDone = true;
			}

			else if (playerList[hi.effectPID]->GetConsecutiveFrags() == 5 && !playerList[hi.effectPID]->impressiveDone)
			{
				console->printMsg("Impressive, " + playerList[hi.effectPID]->getName(), "System", 'S');
				if (GetSoundActivated() && hi.effectPID == localPlayerId)
				{
					GetSound()->playUserGeneratedSound(SOUNDS::announcerImpressive);
				}
				playerList[hi.effectPID]->impressiveDone = true;
			}

			glm::vec3 hpPos = p->getPos();
			hpPos.y += 0.5f;
			addEffectToList(-1, p->getTeam(), hi.playerHit, EFFECT_TYPE::HEALTHPACK, hpPos, 0, 0.5f);

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

		if (d < 0.3f)
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
	decalAdd(hi, 0.35f);

	int arraypos = -1;
	Bullet* b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, arraypos);
	if (b != nullptr)
	{
		b->setPos(hi.hitPos);
		b->setDir(hi.hitDir);
		if (b != nullptr)
		{
			vec3 temp;
			vec3 vel = b->getVel();

			switch (hi.bt)
			{
			case BULLET_TYPE::CLUSTER_GRENADE:
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				vel *= 0.7f;
				b->setVel(vel);
				bounceBullet(hi, b);
				if (b->getBounces() > 3)
					removeBullet(hi.bt, arraypos);
				temp = b->getDir();
				temp.x *= 0.8f;
				temp.y *= 0.6f;
				temp.z *= 0.8f;
				b->setDir(temp);
				break;
			case BULLET_TYPE::CLUSTERLING:
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
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
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				vel *= 0.7f;
				b->setVel(vel);
				bounceBullet(hi, b);
				if (b->getBounces() > 3)
					removeBullet(hi.bt, arraypos);
				temp = b->getDir();
				temp.x *= 0.8f;
				temp.y *= 0.6f;
				temp.z *= 0.8f;
				b->setDir(temp);
				break;
			case BULLET_TYPE::VACUUM_GRENADE:
				vel *= 0.7f;
				b->setVel(vel);
				bounceBullet(hi, b);
				temp = b->getDir();
				temp.x *= 0.8f;
				temp.y *= 0.6f;
				temp.z *= 0.8f;
				b->setDir(temp);
				break;
			case BULLET_TYPE::DISC_SHOT:
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				bounceBullet(hi, b);
				break;
			case BULLET_TYPE::GRENADE_SHOT:
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncherBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				vel *= 0.7f;
				b->setVel(vel);
				bounceBullet(hi, b);
				temp = b->getDir();
				temp.x *= 0.8f;
				temp.y *= 0.6f;
				temp.z *= 0.8f;
				if (b->getBounces() > 3)
					removeBullet(hi.bt, arraypos);
				b->setDir(temp);
				break;
			default:
				removeBullet(hi.bt, arraypos);
				break;
			}
		}
	}
}

void Game::handleBulletHitEffectEvent(BulletHitEffectInfo hi)
{
	int arraypos = -1;
	Bullet* b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, arraypos);
	if (b != nullptr)
	{
		//Add exceptions 4 battery fields
		removeBullet(hi.bt, arraypos);
		/*vec3 temp;
		switch (hi.bt)
		{
		case BULLET_TYPE::PULSE_SHOT:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::PLASMA_SHOT:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::GRENADE_SHOT:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::SHOTGUN_PELLET:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::THERMITE_GRENADE:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::CLEANSE_BOMB:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::CLUSTER_GRENADE:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::CLUSTERLING:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::BATTERY_SLOW_SHOT:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::BATTERY_SPEED_SHOT:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::LINK_SHOT:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::VACUUM_GRENADE:
			bounceBullet(forBounce, b);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
			break;
		case BULLET_TYPE::DISC_SHOT:
			bounceBullet(forBounce, b);
			break;
		case BULLET_TYPE::HACKING_DART:
			removeBullet(hi.bt, arraypos);
			break;
		case BULLET_TYPE::MELEE_ATTACK:
			removeBullet(hi.bt, arraypos);
			break;
		}*/
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
				GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z);
			break;
		}
		case BULLET_TYPE::CLUSTERLING:
		{
			addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 10, 3.0f);

			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectClusterlingExplosion, parent->getPos().x, parent->getPos().y, parent->getPos().z);
			break;
		}
		case BULLET_TYPE::CLEANSE_BOMB:
		{
			addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 0, 4.0f);
			break;
		}
		case BULLET_TYPE::VACUUM_GRENADE:
		{
			addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 10, 4.0f);
			break;
		}
		case BULLET_TYPE::THERMITE_GRENADE:
		{
			addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::THERMITE_CLOUD, parent->getPos(), 10, 0.0f);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectThermiteGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z);
			break;
		}
		case BULLET_TYPE::GRENADE_SHOT:
		{
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z);

			addEffectToList(PID, parent->getTeam(), BID, EFFECT_TYPE::EXPLOSION, parent->getPos(), 15, 3.5f);
			break;
		}
		}
		delete bullets[bt][posInArray];
		bullets[bt][posInArray] = bullets[bt][bullets[bt].size() - 1];
		bullets[bt].pop_back();
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
	decals_renderInfo[decalCounter].pos = info.hitPos - vec3(info.collisionNormal) * (rad - info.collisionNormal.w);

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