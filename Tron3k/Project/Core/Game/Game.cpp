#include "Game.h"

void Game::release()
{
	// delete code goes here
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

	playerList = new Player*[max_con];
	for (int c = 0; c < max_con; c++)
		playerList[c] = nullptr;

	freecam = false;
	spectateID = -1;
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

void Game::update(float dt)
{
	//some things need to be done before movement, some after
	if(gameState != SERVER)
		console->setInChatMode(playerList[localPlayerId]->getLockedControls());

	if (gameState == Gamestate::ROAM)
	{
		checkPlayerVEffectCollision();
	}

	if (gameState == Gamestate::CLIENT)
	{
		checkPlayerVEffectCollision();
	}

	if (gameState == Gamestate::SERVER)
	{
		checkPlayerVEffectCollision();
	}

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

	if (gameState == Gamestate::ROAM)
	{
		checkPlayerVWorldCollision(dt);
		checkBulletVWorldCollision();
		checkPlayerVEffectCollision();
		checkBulletVEffectCollision();
	}

	if (gameState == Gamestate::CLIENT)
	{
		checkPvPCollision();
		checkPlayerVWorldCollision(dt);
		checkPlayerVEffectCollision();
		checkFootsteps(dt);
	}

	if (gameState == Gamestate::SERVER)
	{
		checkBulletVWorldCollision();
		checkPlayerVBulletCollision();
		checkPlayerVEffectCollision();
		checkBulletVEffectCollision();
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





	for (unsigned int i = 0; i < EFFECT_TYPE::NROFEFFECTS; i++)
	{
		for (unsigned int c = 0; c < effects[i].size(); c++)
		{
			int msg = effects[i][c]->update(dt);

			if (i == EFFECT_TYPE::LIGHT_WALL)
			{
				updateEffectBox(effects[i][c]);

			}

			if (msg == 1)		//Effect is dead
			{
				//effect removal in physics
				int pid = -1, eid = -1;
				effects[i][c]->getId(pid, eid);
				physics->removeEffect(eid);

				removeEffect(EFFECT_TYPE(i), c);
			}

		}
	}
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
		freecam = true;
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
	playerList[conID] = new Player();
	playerList[conID]->init(p->getName(), p->getPos(), isLocal);
	playerList[conID]->setTeam(p->getTeam());
	playerList[conID]->setHP(hp);
	playerList[conID]->setRole(*templateRole);
	playerList[conID]->getRole()->chooseRole(role);

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
	physics->receiveWorldBoxes(wBoxes);
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
			physics->removeEffect(eid);
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

			if (playerList[i]->getFootsteps())
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
	std::vector<glm::vec4> collNormals;
	if (gameState == Gamestate::ROAM || gameState == Gamestate::CLIENT)
	{
		Player* local = playerList[localPlayerId];
		//Collision for all wall-like effects i.e. only Lightwall and Thunderdome

		std::vector<glm::vec4> collNormalWalls;
		std::vector<glm::vec4> collNormalDomes;

		for (int c = 0; c < effects[EFFECT_TYPE::LIGHT_WALL].size(); c++)
		{
			int eid = -1, pid = -1;
			effects[EFFECT_TYPE::LIGHT_WALL][c]->getId(pid, eid);
			if (((LightwallEffect*)effects[EFFECT_TYPE::LIGHT_WALL][c])->getCollidable() || localPlayerId != pid)
			{
				collNormalWalls = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::LIGHT_WALL, eid);
				collNormals.reserve(collNormalWalls.size()); // preallocate memory
				collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
				collNormalWalls.clear();
			}

		}

		for (int c = 0; c < effects[EFFECT_TYPE::THUNDER_DOME].size(); c++)
		{
			int eid = -1, pid = -1;
			effects[EFFECT_TYPE::THUNDER_DOME][c]->getId(pid, eid);
			collNormalDomes = physics->checkPlayerVEffectCollision(local->getPos(), EFFECT_TYPE::THUNDER_DOME, eid);

			glm::vec3 vel = local->getVelocity();
			collNormals.reserve(collNormalDomes.size()); // preallocate memory
			collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());
			collNormalDomes.clear();
		}

		/*//merge normals into one vector
		collNormals.reserve(collNormalWalls.size() + collNormalDomes.size()); // preallocate memory
		collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
		collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());*/

		if (collNormals.size() > 0)
			playerList[localPlayerId]->setCollisionInfo(collNormals);
	}
	if (gameState == Gamestate::ROAM || gameState == Gamestate::SERVER)
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
						if ((bullets[b][j]->getTeamId() != playerList[i]->getTeam() && playerList[i]->getTeam() != 0) || (playerList[pid]->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER) && pid != i)) //Don't shoot same team, don't shoot spectators
						{
							if (playerList[i]->isAlive()) //Don't shoot dead people
								collides = physics->checkPlayerVBulletCollision(playerList[i]->getPos() - (vec3(0, playerList[i]->getRole()->getBoxModifier(), 0)), bullets[b][j]->getPos());
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
							return;
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
		std::vector<vec4> cNorms = physics->PlayerVWorldCollision(playerList[localPlayerId]->getPos() - (vec3(0, playerList[localPlayerId]->getRole()->getBoxModifier(), 0)));


		//if we collided with something
		if (cNorms.size() > 0)
		{
			playerList[localPlayerId]->setCollisionInfo(cNorms);

			//Rest is handled by player
		}
	}
}

void Game::checkBulletVWorldCollision()
{
	std::vector<glm::vec4> collides;

	for (unsigned int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
	{
		for (unsigned int j = 0; j < bullets[b].size(); j++)
		{
			if (bullets[b][j] != nullptr)
			{
				int pid = -1, bid = -1;
				bullets[b][j]->getId(pid, bid);
				collides = physics->BulletVWorldCollision(bullets[b][j]->getPos());
				if (collides.size() > 0)
				{
					BulletHitWorldInfo hi;
					hi.bt = BULLET_TYPE(b); hi.hitPos = bullets[b][j]->getPos(); hi.hitDir = bullets[b][j]->getDir();
					bullets[b][j]->getId(hi.bulletPID, hi.bulletBID);
					hi.collisionNormal = collides[0];
					allBulletHitsOnWorld.push_back(hi);
				}
			}
		}
	}
}

void Game::checkBulletVEffectCollision()
{
	std::vector<glm::vec4> collNormals;
	if (gameState == Gamestate::ROAM || gameState == Gamestate::SERVER)
	{
		//Collision for all wall-like effects i.e. only Lightwall and Thunderdome

		std::vector<glm::vec4> collNormalWalls;
		std::vector<glm::vec4> collNormalDomes;

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
						collNormalWalls = physics->checkBulletVEffectCollision(bullets[b][j]->getPos(), EFFECT_TYPE::LIGHT_WALL, eid);
						collNormals.reserve(collNormalWalls.size()); // preallocate memory
						collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
						collNormalWalls.clear();
						if (collNormals.size() > 0)
						{
							int tuttar = 2;
							BulletHitEffectInfo bi;
							bullets[b][j]->getId(bi.bulletPID, bi.bulletBID);
							bi.bt = BULLET_TYPE(b);
							bi.et = EFFECT_TYPE::LIGHT_WALL;
							bi.hitPos = bullets[b][j]->getPos();
							bi.hitDir = bullets[b][j]->getDir();
							bi.collisionNormal = collNormals[0];
							allBulletHitsOnEffects.push_back(bi);
							collNormals.clear();
						}
					}

				}
				for (int c = 0; c < effects[EFFECT_TYPE::THUNDER_DOME].size(); c++)
				{
					int eid = -1, pid = -1;
					effects[EFFECT_TYPE::THUNDER_DOME][c]->getId(pid, eid);
					collNormalDomes = physics->checkPlayerVEffectCollision(bullets[b][j]->getPos(), EFFECT_TYPE::THUNDER_DOME, eid);

					//glm::vec3 vel = bullets[b][j]->getPos()->getVelocity();
					collNormals.reserve(collNormalDomes.size()); // preallocate memory
					collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());
					collNormalDomes.clear();
				}
			}
		}

		/*//merge normals into one vector
		collNormals.reserve(collNormalWalls.size() + collNormalDomes.size()); // preallocate memory
		collNormals.insert(collNormals.end(), collNormalWalls.begin(), collNormalWalls.end());
		collNormals.insert(collNormals.end(), collNormalDomes.begin(), collNormalDomes.end());*/

		if (collNormals.size() > 0)
			playerList[localPlayerId]->setCollisionInfo(collNormals);
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

void Game::addPlayerToTeam(int p_conID, int team, int spawnPosition)
{
	switch (team)
	{
	case 0:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamSpectators.push_back(p_conID);
		playerList[p_conID]->setTeam(0);
		break;
	case 1:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamOne.push_back(p_conID);
		playerList[p_conID]->setTeam(1);
		allowPlayerRespawn(p_conID, spawnPosition);

		//if (playerList[p_conID]->isLocal())
		//{

		//}
		break;
	case 2:
		removeConIDfromTeams(p_conID);
		//if (playerList[p_conID] != nullptr)
		//	delete playerList[p_conID];
		teamTwo.push_back(p_conID);
		playerList[p_conID]->setTeam(2);
		allowPlayerRespawn(p_conID, spawnPosition);
		break;
	}
}

void Game::getLatestWeaponFired(int localPlayer, WEAPON_TYPE &wt, int &bulletId)
{
	shotsFired = false;
	Player* p = playerList[localPlayer];
	handleWeaponFire(localPlayer, bulletShot, weaponShotWith, p->getPos(), p->getDir());
	wt = weaponShotWith;
	bulletId = bulletShot;
	bulletShot = -1;
}

void Game::addBulletToList(int conID, int bulletId, BULLET_TYPE bt, glm::vec3 pos, glm::vec3 dir)
{
	Bullet* b = nullptr;
	Player* p = playerList[conID];
	glm::vec3 rightV = normalize(cross(dir, vec3(0, 1, 0)));
	glm::vec3 upV = normalize(cross(rightV, dir));
	glm::vec3 dirMod = dir;
	switch (bt)
	{
	case BULLET_TYPE::PULSE_SHOT:
		rightV *= 0.65f;
		upV *= -0.17f;
		dirMod *= 2.4f;
		pos += upV + rightV + dirMod;
	
		b = new PulseShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::PLASMA_SHOT:
		b = new PlasmaShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::GRENADE_SHOT:
		b = new GrenadeShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::SHOTGUN_PELLET:
		b = new ShotgunPellet(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::THERMITE_GRENADE:
		b = new ThermiteGrenade(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::CLEANSE_BOMB:
		b = new CleanseBomb(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::CLUSTER_GRENADE:
		b = new ClusterGrenade(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::CLUSTERLING:
		b = new Clusterling(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::BATTERY_SLOW_SHOT:
		b = new BatterySlowShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::BATTERY_SPEED_SHOT:
		b = new BatterySpeedShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::LINK_SHOT:
		b = new LinkShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::VACUUM_GRENADE:
		b = new VacuumGrenade(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::DISC_SHOT:
		b = new DiscShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	case BULLET_TYPE::HACKING_DART:
		b = new HackingDart(pos, dir, conID, bulletId, p->getTeam(), BULLET_TYPE::HACKING_DART);
		break;
	case BULLET_TYPE::MELEE_ATTACK:
		b = new MeleeAttack(pos, dir, conID, bulletId, p->getTeam(), BULLET_TYPE::MELEE_ATTACK);
		break;
	}

	bullets[bt].push_back(b);

}

void Game::handleWeaponFire(int conID, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir)
{
	switch (weapontype)
	{
	case WEAPON_TYPE::PULSE_RIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::PULSE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::ENERGY_BOOST:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectEnergyBoost, pos.x, pos.y, pos.z);
		playerList[conID]->healing(10);
		break;

	case WEAPON_TYPE::PLASMA_AUTORIFLE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::PLASMA_SHOT, pos, dir);
		break;
	case WEAPON_TYPE::ENERGY_SHIELD:
		if (GetSound())
			GetSound()->playExternalSound(SOUNDS::soundEffectShield, pos.x, pos.y, pos.z);
		break;

	case WEAPON_TYPE::DISC_GUN:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectDiscGun, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::DISC_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::MELEE:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectMelee, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::MELEE_ATTACK, pos, dir);
		break;

	case WEAPON_TYPE::GRENADE_LAUNCHER:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeLauncher, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::GRENADE_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::SHOTGUN:
	{
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectShotGun, pos.x, pos.y, pos.z);
		glm::vec3 rightV = normalize(cross(dir, vec3(0, 1, 0)));
		glm::vec3 upV = normalize(cross(dir, rightV));
		for (int k = 0; k < 16; k++)
		{
			//float xrand = (rand() % 50) / 100.0f - 0.25f;
			//float yrand = (rand() % 50) / 100.0f - 0.25f;
			float xoff = glm::sin(k);
			float yoff = glm::cos(k);
			float r = (rand() % 100) / 1000.0f;
			rightV *= xoff*r;
			upV *= yoff*r;
			glm::vec3 ndir = dir + upV + rightV;
			addBulletToList(conID, bulletId, BULLET_TYPE::SHOTGUN_PELLET, pos, ndir);
			rightV = normalize(cross(dir, vec3(0, 1, 0)));
			upV = normalize(cross(dir, rightV));
		}
	}
	break;

	case WEAPON_TYPE::LINK_GUN:
		if (gameState != Gamestate::SERVER)
			if (GetSound())
				GetSound()->playExternalSound(SOUNDS::soundEffectLinkGun, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::LINK_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::BATTERYFIELD_SLOW:
		addBulletToList(conID, bulletId, BULLET_TYPE::BATTERY_SLOW_SHOT, pos, dir);
		break;

	case WEAPON_TYPE::BATTERYFIELD_SPEED:
		addBulletToList(conID, bulletId, BULLET_TYPE::BATTERY_SPEED_SHOT, pos, dir);
		break;
	}
}

CONSUMABLE_TYPE Game::getConsumableUsed(int localPlayer)
{
	consumableUsed = false;
	Player* p = playerList[localPlayer];
	handleConsumableUse(localPlayer, itemUsed, p->getPos(), p->getDir());
	return itemUsed;
}

void Game::handleConsumableUse(int conID, CONSUMABLE_TYPE ct, glm::vec3 pos, glm::vec3 dir)
{
	switch (ct)
	{
	case CONSUMABLE_TYPE::CLUSTERGRENADE:
		addBulletToList(conID, 0, BULLET_TYPE::CLUSTER_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::OVERCHARGE:
		playerList[conID]->addModifier(OVERCHARGEMODIFIER);
		break;
	case CONSUMABLE_TYPE::VACUUMGRENADE:
		addBulletToList(conID, 0, BULLET_TYPE::VACUUM_GRENADE, pos, dir);
		break;
	case CONSUMABLE_TYPE::LIGHTSPEED:
		playerList[conID]->addModifier(LIGHTSPEEDMODIFIER);
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectLightSpeed, pos.x, pos.y, pos.z);
		break;
	case CONSUMABLE_TYPE::THERMITEGRENADE:
		addBulletToList(conID, 0, BULLET_TYPE::THERMITE_GRENADE, pos, dir);
		break;
	}
}

SPECIAL_TYPE Game::getSpecialAbilityUsed(int localPlayer, int &sid)
{
	specialActivated = false;
	Player* p = playerList[localPlayer];
	sid = p->getRole()->getSpecialAbility()->getSpecialId();
	handleSpecialAbilityUse(localPlayer, sid, specialUsed, p->getPos(), p->getDir());
	return specialUsed;
}

void Game::handleSpecialAbilityUse(int conID, int sID, SPECIAL_TYPE st, glm::vec3 pos, glm::vec3 dir)
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
		addEffectToList(conID, sID, EFFECT_TYPE::LIGHT_WALL, pos);

	}
	break;
	case SPECIAL_TYPE::THUNDERDOME:
		if (!p->isLocal())
			p->setDir(dir);
		addEffectToList(conID, sID, EFFECT_TYPE::THUNDER_DOME, pos);
		break;
	case SPECIAL_TYPE::MULTIJUMP:
	{
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectTrapperMultiJump, pos.x, pos.y, pos.z);

		vec3 vel = p->getVelocity();
		if (vel.y < 0)
		{
			vel.y = 8.0f;
		}
		else
		{
			vel.y += 8.0f;
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
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectHunterJump, pos.x, pos.y, pos.z);

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
		addBulletToList(conID, 0, BULLET_TYPE::HACKING_DART, pos, dir);
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectHackingDart, pos.x, pos.y, pos.z);
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
	}
	break;
	}
}

void Game::addEffectToList(int conID, int effectId, EFFECT_TYPE et, glm::vec3 pos)
{
	Effect* e = nullptr;
	Player* p = playerList[conID];

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
		break;
	}
	e->init(conID, effectId, pos);
	effects[et].push_back(e);
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
	glm::vec3 pos = playerList[hi.playerHit]->getPos();
	Player* p = playerList[hi.playerHit];
	if (hi.bt != BULLET_TYPE::CLUSTERLING)	//Any bullets that should not detonate on contact
	{
		if (gameState != Gamestate::SERVER)
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z);
		int bulletPosInArray;
		Bullet* theBullet = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, bulletPosInArray);
		if (theBullet != nullptr)
			p->hitByBullet(theBullet, hi.newHPtotal);

		removeBullet(hi.bt, bulletPosInArray);

	}
	int newHP = p->getHP();
	return newHP;
}

int Game::handleEffectHitPlayerEvent(EffectHitPlayerInfo hi)
{
	glm::vec3 pos = playerList[hi.playerHit]->getPos();
	if (gameState != Gamestate::SERVER)
		if (GetSoundActivated())
			GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z);
	Player* p = playerList[hi.playerHit];
	int effectPosInArray;
	Effect* theEffect = getSpecificEffect(hi.effectPID, hi.effectID, hi.et, effectPosInArray);
	p->hitByEffect(theEffect, hi.newHPtotal);

	removeEffect(hi.et, effectPosInArray);
	int newHP = p->getHP();
	return newHP;
}

void Game::bounceBullet(BulletHitWorldInfo hwi, Bullet* theBullet)
{
	vec4 combinedNormal(0); // all normals added together and then normalized
	vec4 posadjust(0); //pendepths combined

	posadjust += hwi.collisionNormal * hwi.collisionNormal.w;
	combinedNormal += hwi.collisionNormal;

	// the w component holds the pendepth
	vec3 combinedNormal2 = vec3(combinedNormal);

	//reflect!!
	if (combinedNormal2 != glm::vec3(0, 0, 0))
	{
		combinedNormal2 = normalize(combinedNormal2);

		theBullet->setDir(reflect(theBullet->getDir(), combinedNormal2));

		//use pendepth to set a new pos 
		theBullet->setPos(theBullet->getPos() + vec3(posadjust));

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
	int arraypos = -1;
	Bullet* b = getSpecificBullet(hi.bulletPID, hi.bulletBID, hi.bt, arraypos);
	if (b != nullptr)
	{
		b->setPos(hi.hitPos);
		b->setDir(hi.hitDir);
		if (b != nullptr)
		{
			vec3 temp;
			switch (hi.bt)
			{
			case BULLET_TYPE::CLUSTER_GRENADE:
				bounceBullet(hi, b);
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				break;
			case BULLET_TYPE::CLUSTERLING:
				bounceBullet(hi, b);
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectGrenadeBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				break;
			case BULLET_TYPE::THERMITE_GRENADE:
				bounceBullet(hi, b);
				break;
			case BULLET_TYPE::VACUUM_GRENADE:
				bounceBullet(hi, b);
				break;
			case BULLET_TYPE::DISC_SHOT:
				bounceBullet(hi, b);
				if (GetSoundActivated())
					GetSound()->playExternalSound(SOUNDS::soundEffectDiscBounce, hi.hitPos.x, hi.hitPos.y, hi.hitPos.z);
				break;
			case BULLET_TYPE::GRENADE_SHOT:
				bounceBullet(hi, b);
				temp = b->getVel();
				temp.x *= 0.6;
				temp.y *= 0.6;
				temp.z *= 0.6;
				b->setVel(temp);
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
	//Jag hatar structs ... please do better
	BulletHitWorldInfo forBounce; forBounce.bulletPID = hi.bulletPID; forBounce.bulletBID = hi.bulletBID; forBounce.bt = hi.bt; forBounce.hitPos = hi.hitPos; forBounce.hitDir = hi.hitDir; forBounce.collisionNormal = hi.collisionNormal;
	if (b != nullptr)
	{
		vec3 temp;
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
		}
	}
}

void Game::handleEffectHitEffectEvent(EffectHitEffectInfo hi)
{
	int arraypos = -1;
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
	}
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

void Game::removeEffect(EFFECT_TYPE et, int posInArray)
{
	switch (et)
	{
	case EFFECT_TYPE::LIGHT_WALL:
		break;
	}
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
			addBulletToList(PID, BID, CLUSTERLING, parent->getPos(), lingDir);
			lingDir.x = -lingDir.x;
			addBulletToList(PID, BID + 1, CLUSTERLING, parent->getPos(), lingDir);
			lingDir.z = -lingDir.z;
			addBulletToList(PID, BID + 2, CLUSTERLING, parent->getPos(), lingDir);
			lingDir.x = -lingDir.x;
			addBulletToList(PID, BID + 3, CLUSTERLING, parent->getPos(), lingDir);

			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(20);

			//This is where you send it to physics
			std::vector<float> eBox;
			eBox.push_back(parent->getPos().x);
			eBox.push_back(parent->getPos().y);
			eBox.push_back(parent->getPos().z);
			eBox.push_back(20);

			int pid, eid;
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->getId(pid, eid);

			physics->receiveEffectBox(eBox, EFFECT_TYPE::EXPLOSION, pid, eid);

			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectClusterGrenade, parent->getPos().x, parent->getPos().y, parent->getPos().z);
			break;
		}
		case BULLET_TYPE::CLUSTERLING:
		{
			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(10.0f);
			if (GetSoundActivated())
				GetSound()->playExternalSound(SOUNDS::soundEffectClusterlingExplosion, parent->getPos().x, parent->getPos().y, parent->getPos().z);

			//This is where you send it to physics
			std::vector<float> eBox;
			eBox.push_back(parent->getPos().x);
			eBox.push_back(parent->getPos().y);
			eBox.push_back(parent->getPos().z);
			eBox.push_back(20);

			int pid, eid;
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->getId(pid, eid);

			physics->receiveEffectBox(eBox, EFFECT_TYPE::EXPLOSION, pid, eid);
			break;
		}
		case BULLET_TYPE::CLEANSE_BOMB:
		{
			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(15.0f);
			break;
		}
		case BULLET_TYPE::VACUUM_GRENADE:
		{
			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(15.0f);
			break;
		}
		case BULLET_TYPE::THERMITE_GRENADE:
		{
			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(35.0f);
			break;
		}
		case BULLET_TYPE::GRENADE_SHOT:
		{
			addEffectToList(PID, BID, EFFECT_TYPE::EXPLOSION, parent->getPos());
			effects[EFFECT_TYPE::EXPLOSION][effects[EFFECT_TYPE::EXPLOSION].size() - 1]->setInterestingVariable(35.0f);
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
	playerList[p_conID]->respawn(spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].pos, spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].dir, spawnpoints[playerList[p_conID]->getTeam()][respawnPosition].roomID);
	localPlayerWantsRespawn = false;
	localPlayerRespawnWaiting = false;
	if (playerList[p_conID]->isLocal())
		freecam = false;
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
