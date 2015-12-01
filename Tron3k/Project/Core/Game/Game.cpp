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
		for (int i = 0; i < bullets[c].size(); i++)
		{
			if (bullets[c][i] != nullptr)
			{
				delete bullets[c][i];
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

void Game::init(int max_connections, int state)
{
	max_con = max_connections;

	loadRoles();
	initPhysics();

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
	for (int c = 0; c < max_con; c++)
	{
		if (playerList[c] != nullptr)
		{
			bool spectatingThis = false;
			bool spectating = false;
			if (spectateID > -1)
			{
				spectating = true;
				if (c == spectateID)
					spectatingThis = true;
			}

			if (playerList[c]->isLocal())
			{
				if(!playerList[c]->getGrounded())
				playerList[c]->applyGravity(physics, dt);
			}

			PLAYERMSG msg = playerList[c]->update(dt, freecam, spectatingThis, spectating);
			if (msg == PLAYERMSG::SHOOT)
			{
				//if (gameState != Gamestate::ROAM)
					registerWeapon(playerList[c]);
				/*else
				{
					registerWeapon(playerList[c]);
					handleWeaponFire(c, bulletShot, weaponShotWith, playerList[c]->getPos(), playerList[c]->getDir());
				}*/
			}
			if (msg == PLAYERMSG::WPNSWITCH)
			{
				registerSwitch(playerList[c]);
			}

			if (msg == PLAYERMSG::DEATH)
			{
				freecam = true;
			}
			if (msg == PLAYERMSG::PLAYERRESPAWN)
			{
				if (playerList[c]->isLocal())
					localPlayerWantsRespawn = true;
			}
		}
	}

	for (int i = 0; i < BULLET_TYPE::NROFBULLETS; i++)
	{
		for (int c = 0; c < bullets[i].size(); c++)
		{
			int msg = bullets[i][c]->update(dt);
			if (msg == 1)		//Bullet is dead
			{
				delete bullets[i][c];
				bullets[i][c] = bullets[i][bullets[i].size() - 1];
				bullets[i].pop_back();
			}
		}
	}

	if (gameState == Gamestate::ROAM)
	{
		checkPlayerVWorldCollision();
	}

	if (gameState == Gamestate::CLIENT)
	{
		checkPvPCollision();
		checkPlayerVWorldCollision();
	}

	if (gameState == Gamestate::SERVER)
		checkPlayerVBulletCollision();
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

void Game::createPlayer(Player* p, int conID, bool isLocal)
{
	playerList[conID] = new Player();
	playerList[conID]->init(p->getName(), p->getPos(), isLocal);
	playerList[conID]->setTeam(p->getTeam());
	playerList[conID]->setRole(*templateRole);
	if (isLocal)
		freecam = true;
}

void Game::removePlayer(int conID)
{
	delete playerList[conID];
	playerList[conID] = nullptr;
	removeConIDfromTeams(conID);
}

void Game::sendPlayerBox(std::vector<float> pBox)
{
	physics->receivePlayerBox(pBox);
}

void Game::sendWorldBoxes(std::vector<std::vector<float>> wBoxes)
{
	physics->receiveWorldBoxes(wBoxes);
}

void Game::checkPvPCollision()
{
	//TEMPORARY
	vec3 pPos[20];
	vec3 localPPos = vec3(0, 0, 0);
	int localP = -1;
	int localPTeam = -1;
	for (int i = 0; i < max_con; i++)
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
	bool collides = false;
	for (int i = 0; i < max_con; i++)
	{
		collides = false;
		if (playerList[i] != nullptr)
			collides = physics->checkPlayerVPlayerCollision(localPPos, pPos[i]);

		if (collides)
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

void Game::checkPlayerVBulletCollision()
{
	bool collides = false;

	for (int i = 0; i < max_con; i++)
	{
		Player* p = playerList[i];
		if (p != nullptr)
		{
			for (int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
			{
				for (int j = 0; j < bullets[b].size(); j++)
				{
					collides = false;
					if (bullets[b][j] != nullptr)
					{
						int pid = -1, bid = -1;
						bullets[b][j]->getId(pid, bid);
						if (bullets[b][j]->getTeamId() != playerList[i]->getTeam() && playerList[i]->getTeam() != 0) //Don't shoot same team, don't shoot spectators
						{
							if (playerList[i]->isAlive()) //Don't shoot dead people
								collides = physics->checkPlayerVBulletCollision(playerList[i]->getPos(), bullets[b][j]->getPos());
						}
						if (collides)
						{
							//Code for player on bullet collision goes here
							/*

							Spara undan player som blev tr�ffad (dvs i), bulletId (pID och bID) och BULLET_TYPE. �ven dir.
							L�gg f�rslagsvis i en vector<struct> som kan skickas till Core. Core matar in alla tr�ffade spelare i ett NET_EVENT (conID, bID, pID, bt).
							NET_EVENT eftersom allt detta sker endast p� servern
							Tas emot av klienterna, Core->Game. Game hanterar sina bullethits n�r paketet tas emot.

							Servern g�r givetvis samma ber�kning f�r bullethits (speed/position-data pga knockbacks antingen g�rs inte eller g�rs och sedan discardas pga client->server update)
							direkt n�r kollisionen sker, dvs.

							*/

							hit.bt = BULLET_TYPE(b);
							hit.playerHit = i;
							bullets[b][j]->getId(hit.bulletPID, hit.bulletBID);

							playerHit = true;
							return;
						}
					}
				}
			}
		}
		
	}
}

void Game::checkPlayerVWorldCollision()
{
	bool collides = false;
	for (int i = 0; i < max_con; i++)
	{
		if (playerList[i] != nullptr)
		{
			if (playerList[i]->isLocal())
			{
				collides = physics->checkPlayerVWorldCollision(playerList[i]->getPos());
				playerList[i]->setGrounded(collides);
				if (collides)
				{
					//collision with world here, no gravity etc
					glm::vec3 vel = playerList[i]->getVelocity();
					glm::vec3 pos = playerList[i]->getPos();
					pos.y += vel.y;
					playerList[i]->setGoalPos(pos);
					vel.y = 0.0f;
					playerList[i]->setVelocity(vel);
				}
			}
		}
	}
}

void Game::checkBulletVWorldCollision()
{
	bool collides = false;

	for (int b = 0; b < BULLET_TYPE::NROFBULLETS; b++)
	{
		for (int j = 0; j < bullets[b].size(); j++)
		{
			collides = false;
			if (bullets[b][j] != nullptr)
			{
				int pid = -1, bid = -1;
				bullets[b][j]->getId(pid, bid);
				collides = physics->checkBulletVWorldCollision(bullets[b][j]->getPos());
				if (collides)
				{
					//TODO: remove bullet, add network thing for deleting the bullet
					//handleBulletHitEvent()
				}
			}
		}
	}
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
	wpnSwitched = true;
}

void Game::handleWeaponSwitch(int conID, WEAPON_TYPE ws)
{
	playerList[conID]->switchWpn(ws);
}

int Game::getPlayersOnTeam(int team)
{
	switch (team)
	{
	case 0: return teamSpectators.size();
	case 1: return teamOne.size();
	case 2: return teamTwo.size();
	}
}

void Game::removeConIDfromTeams(int conID)
{
	for (int c = 0; c < teamSpectators.size(); c++)
	{
		if (teamSpectators[c] == conID)
		{
			teamSpectators[c] = teamSpectators[teamSpectators.size() - 1];
			teamSpectators.pop_back();
			return;
		}
	}
	for (int c = 0; c < teamOne.size(); c++)
	{
		if (teamOne[c] == conID)
		{
			teamOne[c] = teamOne[teamOne.size() - 1];
			teamOne.pop_back();
			return;
		}
	}
	for (int c = 0; c < teamTwo.size(); c++)
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

	switch (bt)
	{
	case BULLET_TYPE::PULSE_SHOT:
		b = new PulseShot(pos, dir, conID, bulletId, p->getTeam());
		break;
	}
	
	bullets[bt].push_back(b);
	
}

void Game::handleWeaponFire(int conID, int bulletId, WEAPON_TYPE weapontype, glm::vec3 pos, glm::vec3 dir)
{
	switch (weapontype)
	{
	case WEAPON_TYPE::PULSE_RIFLE:
		//To do: Automate generation of bullets
		if (gameState != Gamestate::SERVER)
			GetSound()->playExternalSound(SOUNDS::soundEffectPusleRifleShot, pos.x, pos.y, pos.z);
		addBulletToList(conID, bulletId, BULLET_TYPE::PULSE_SHOT, pos, dir);
		break;
	case WEAPON_TYPE::ENERGY_BOOST:
		if (gameState != Gamestate::SERVER)
			GetSound()->playExternalSound(SOUNDS::soundEffectPoopRifleShot, pos.x, pos.y, pos.z);
		playerList[conID]->healing(10);
		break;
	}
}

Bullet* Game::getBulletForRemoval(int PID, int BID, BULLET_TYPE bt, int &posInBulletArray)
{
	for (int c = 0; c < bullets[bt].size(); c++)
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
}

int Game::handleBulletHitEvent(BulletHitInfo hi, int newHPtotal)
{
	glm::vec3 pos = playerList[hi.playerHit]->getPos();
	if (gameState != Gamestate::SERVER)
		GetSound()->playExternalSound(SOUNDS::soundEffectBulletPlayerHit, pos.x, pos.y, pos.z);
	Player* p = playerList[hi.playerHit];
	int bulletPosInArray;
	Bullet* theBullet = getBulletForRemoval(hi.bulletPID, hi.bulletBID, hi.bt, bulletPosInArray);
	p->hitByBullet(theBullet, newHPtotal);

	delete bullets[hi.bt][bulletPosInArray];
	bullets[hi.bt][bulletPosInArray] = bullets[hi.bt][bullets[hi.bt].size() - 1];
	bullets[hi.bt].pop_back();

	int newHP = p->getHP();
	return newHP;
}

bool Game::playerWantsToRespawn()
{
	if (localPlayerRespawnWaiting)
		return false;
	else if (localPlayerWantsRespawn)
		localPlayerRespawnWaiting = true;
	return localPlayerWantsRespawn;
}

void Game::allowPlayerRespawn(int p_conID, glm::vec3 respawnPosition)
{
	playerList[p_conID]->respawn(respawnPosition);
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