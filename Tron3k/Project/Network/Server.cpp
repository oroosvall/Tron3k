#include "Server.h"

void Server::disconnected(Uint8 _conID)
{
	if (gamePtr->spectateID == _conID)
		gamePtr->clearSpectateID();

	Player* p = gamePtr->getPlayer(_conID);
	if (p != nullptr)
	{
		con[_conID].disconnect();
		nrConnected--;

		consolePtr->printMsg("Player (" + p->getName() + ") Disconnected", "System", 'S');
		gamePtr->removePlayer(_conID);

		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::PLAYER_LEFT) << _conID;
		branch(out, _conID);
		delete out;
	}
	else
		consolePtr->printMsg("ERROR Disconnect", "System", 'S');
}

void Server::event_player_data()
{
	Packet* out = new Packet();
	Uint8 HP;
	bool shouldSend = false;
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::PLAYERDATA);
	for (int c = 0; c < 20; c++)
	{
		Player* p = gamePtr->getPlayer(c);
		if (p != nullptr)
		{
			shouldSend = true;
			HP = p->getHP();
			*out << HP;
		}
	}
	if (shouldSend)
		branch(out, -1);
	delete out;
}

void Server::event_gamemode_data()
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::GAMEMODE_DATA);
	Gamemode* gm = gamePtr->getGameMode();
	GAMEMODE_TYPE type = gm->getType();
	*out << Uint8(type);
	if (type == GAMEMODE_TYPE::KOTH)
	{
		KingOfTheHill* koth = (KingOfTheHill*)gm;
		Uint8 teamOneTokens = koth->getRespawnTokens(1);
		Uint8 teamTwoTokens = koth->getRespawnTokens(2);
		Uint8 teamOneOnCapP = koth->getPlayersOnPoint(1);
		Uint8 teamTwoOnCapP = koth->getPlayersOnPoint(2);
		Uint8 capPoint = koth->getCapturePoint();
		float timer = koth->getTimer();
		Uint8 teamOneScore = koth->getRoundWins(1);
		Uint8 teamTwoScore = koth->getRoundWins(2);
		Uint8 state = koth->getState();
		Uint8 lastMsg = koth->getLastMsg();
		*out << teamOneTokens << teamTwoTokens << teamOneOnCapP << teamTwoOnCapP << capPoint << timer << teamOneScore << teamTwoScore;
		*out << state << lastMsg;
	}
	
	branch(out, -1);
	delete out;
}

void Server::event_bullet_hit_player(std::vector<BulletHitPlayerInfo> allhits)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::PLAYER_HIT);
	*out << Uint8(allhits.size());
	for (unsigned int c = 0; c < allhits.size(); c++)
	{
		*out << Uint8(allhits[c].playerHit) << Uint8(allhits[c].bulletPID) << Uint8(allhits[c].bulletBID) << Uint8(allhits[c].bt) << Uint8(allhits[c].newHPtotal);
		*out << allhits[c].hitDir.x << allhits[c].hitDir.y << allhits[c].hitDir.z;
	}
	
	branch(out, -1);
	delete out;
}

void Server::event_bullet_hit_effect(std::vector<BulletHitEffectInfo> allhits)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::BULLET_EFFECT_HIT);
	*out << Uint8(allhits.size());
	for (unsigned int c = 0; c < allhits.size(); c++)
	{
		*out << Uint8(allhits[c].bulletPID) << Uint8(allhits[c].bulletBID) << Uint8(allhits[c].bt) << Uint8(allhits[c].bulletTeam);
		*out << Uint8(allhits[c].effectPID) << Uint8(allhits[c].effectID) << Uint8(allhits[c].et);
		*out << allhits[c].hitPos.x << allhits[c].hitPos.y << allhits[c].hitPos.z;
		*out << allhits[c].hitDir.x << allhits[c].hitDir.y << allhits[c].hitDir.z;
		*out << allhits[c].collisionNormal.x << allhits[c].collisionNormal.y;
		*out << allhits[c].collisionNormal.z << allhits[c].collisionNormal.w;
	}

	branch(out, -1);
	delete out;
}

void Server::event_bullet_hit_world(std::vector<BulletHitWorldInfo> allhits)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::BULLET_WORLD_HIT);
	*out << Uint8(allhits.size());
	for (unsigned int c = 0; c < allhits.size(); c++)
	{
		*out << Uint8(allhits[c].bulletPID) << Uint8(allhits[c].bulletBID);
		*out << Uint8(allhits[c].bt);
		*out << allhits[c].hitPos.x << allhits[c].hitPos.y << allhits[c].hitPos.z;
		*out << allhits[c].hitDir.x << allhits[c].hitDir.y << allhits[c].hitDir.z;
		*out << allhits[c].collisionNormal.x << allhits[c].collisionNormal.y <<
			allhits[c].collisionNormal.z << allhits[c].collisionNormal.w;
	}

	branch(out, -1);
	delete out;
}

void Server::event_effect_hit_player(std::vector<EffectHitPlayerInfo> allhits)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::EFFECT_PLAYER_HIT);
	*out << Uint8(allhits.size());
	for (unsigned int c = 0; c < allhits.size(); c++)
	{
		*out << Uint8(allhits[c].playerHit) << Uint8(allhits[c].effectPID) << Uint8(allhits[c].effectID);
		*out << Uint8(allhits[c].et);
		*out << allhits[c].hitPos.x << allhits[c].hitPos.y << allhits[c].hitPos.z;
		*out << allhits[c].playerPos.x << allhits[c].playerPos.y << allhits[c].playerPos.z;
		*out << Uint8(allhits[c].newHPtotal);
	}

	branch(out, -1);
	delete out;
}

void Server::event_bullet_timed_out(std::vector<BulletTimeOutInfo> allbullets)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::BULLET_TIMEOUT);
	*out << Uint8(allbullets.size());
	for (unsigned int c = 0; c < allbullets.size(); c++)
	{
		*out << Uint8(allbullets[c].bt);
		*out << Uint8(allbullets[c].bulletPID) << Uint8(allbullets[c].bulletBID);
		*out << allbullets[c].pos.x << allbullets[c].pos.y << allbullets[c].pos.z;
	}

	branch(out, -1);
	delete out;
}

void Server::event_effect_timed_out(std::vector<EffectTimeOutInfo> alleffects)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::EFFECT_TIMEOUT);
	*out << Uint8(alleffects.size());
	for (unsigned int c = 0; c < alleffects.size(); c++)
	{
		*out << Uint8(alleffects[c].et);
		*out << Uint8(alleffects[c].effectPID) << Uint8(alleffects[c].effectID);
		*out << alleffects[c].pos.x << alleffects[c].pos.y << alleffects[c].pos.z;
	}

	branch(out, -1);
	delete out;
}

Server::~Server()
{
	if (con)
		delete con;
	if (package)
		delete package;
}

void Server::init(Console* console, UIManager* ui, int port, IpAddress addrs, bool record, bool playback)
{
	PORT = port;
	consolePtr = console;
	uiPtr = ui;

	package = new Packet();
	*package << Uint8(NET_INDEX::FRAME);

	isClient = false;

	con = new Connection[MAX_CONNECT];

	for (int n = 0; n < MAX_CONNECT; n++)
		con[n].init();
}

bool Server::bind()
{
	listener.setBlocking(false);
	if (listener.listen(PORT) != sf::Socket::Done)
	{
		return false;
	}
	return true;
}

void Server::branch(Packet* rec, Uint8 conID)
{
	//send to all other clients
	for (int n = 0; n < MAX_CONNECT; n++)
	{
		if (con[n].isConnected() && n != conID)
		{
			con[n].send(rec);
		}
	}
}

bool Server::network_IN(float dt)
{
	//fetch packages from all clients and check for disconnected clients
	for (int n = 0; n < MAX_CONNECT; n++)
	{
		if (con[n].isConnected())
			IN(&con[n], n);
	}

	return true;
}

void Server::network_OUT(float dt)
{
	if (msg_out != "")
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::MESSAGE) << Uint8('S') << scope_out << msg_out;
		branch(out, -1); //-1 sends to all clients
		delete out;

		msg_out = "";
	}

	if (package->getDataSize() > 1) //only send if contains data
	{
		con->send(package);
	}
	package_clear();
}

bool Server::new_connection()
{
	//look for new player connections
	if (nrConnected < MAX_CONNECT)
	{
		//find unused connecton
		Connection* open = 0;
		int counter = 0;
		for (;;)
		{
			if (con[nrConnectedRot].isConnected())
			{
				counter++;
				if (counter >= MAX_CONNECT)
				{
					cout << "server full but nrConnected havn't registered it???" << endl;
				}

				nrConnectedRot++;
				if (nrConnectedRot == MAX_CONNECT)
					nrConnectedRot = 0;
			}
			else
			{
				open = &con[nrConnectedRot];
				break;
			}
		}
		if (open)
		{
			if (listener.accept(*(open->getSocket())) == sf::Socket::Done)
			{
				cout << "new connection established" << endl;
				open->setConnected(true);
				nrConnected++;
			}
		}
	}

	return true;  //isnt used when on server
}

void Server::package_clear()
{
	if (package)
		delete package;
	package = new Packet();
	*package << Uint8(NET_INDEX::EVENT);
}

void Server::in_new_connection(Packet* rec, Uint8 conID)
{
	string pName;
	*rec >> pName;

	Packet* out = new Packet();

	//type of NET_ID
	*out << Uint8(NET_INDEX::NEW_CONNECTION);

	//the player's id
	*out << conID;

	//mapname used
	*out << string("tempMap");

	Player* p = nullptr;
	for (Uint8 c = 0; c < MAX_CONNECT; c++)
	{
		p = gamePtr->getPlayer(c);
		if (p != nullptr)
		{
			*out << Uint8(1);
			*out << p->getName();
			*out << Uint8(p->getTeam());
			*out << Uint8(p->getRole()->getRole());
			*out << Uint8(p->getHP());
		}
		else
		{
			*out << Uint8(0);
			if (c == conID)
			{
				Player* temp = new Player();
				temp->init(pName, glm::vec3(0, 0, 0), gamePtr->getPhysics());
				gamePtr->createPlayer(temp, conID, 100, ROLES::NROFROLES);
				delete temp;
			}
		}
	}

	/*
	SEND COOLDOWNS ON PICKUPS
	*/
	std::vector<Effect*> effects = gamePtr->getEffects(EFFECT_TYPE::HSCPICKUP);
	HSCPickup* hsc = nullptr;
	for (int c = 0; c < effects.size(); c++)
	{
		hsc = (HSCPickup*)effects[c];
		*out << hsc->getCooldown();
	}

	effects = gamePtr->getEffects(EFFECT_TYPE::DOUBLEDAMAGEPICKUP);
	DoubleDamagePickup* dd = nullptr;
	for (int c = 0; c < effects.size(); c++)
	{
		dd = (DoubleDamagePickup*)effects[c];
		*out << dd->getCooldown();
	}

	//reply to the connection
	con[conID].send(out);
	delete out;

	p = gamePtr->getPlayer(conID);
	Packet* b = new Packet;
	*b << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::PLAYER_JOINED);
	*b << conID << p->getName();

	branch(b, conID); //Send information about joined player to all others

	delete b;

	consolePtr->printMsg("Player (" + pName + ") joined the server", "System", 'S');
}

void Server::in_event(Packet* rec, Uint8 conID)
{

}

void Server::in_frame(Packet* rec, Uint8 conID)
{
	Uint8 frame_type;
	while (!rec->endOfPacket())
	{
		*rec >> frame_type;
		switch (frame_type)
		{
		case NET_FRAME::NAME_CHANGE: in_frame_name_change(rec); break;
		case NET_FRAME::POS: in_frame_playerdata(rec); break;
		case NET_FRAME::ANIM: in_frame_anim(rec); break;
		case NET_FRAME::FIRE: in_frame_fire(rec); break;
		case NET_FRAME::WPN_SWITCH: in_frame_weapon_switch(rec); break;
		case NET_FRAME::SPECIAL: in_frame_special_use(rec); break;
		case NET_FRAME::CONSUMABLE: in_frame_consumable(rec); break;
		}
	}

	//branch frame
	branch(rec, conID);
}

void Server::in_message(Packet* rec, Uint8 conID)
{
	string msg_in;
	Uint8 scope_in;
	Uint8 conID_in;

	*rec >> conID_in;
	*rec >> scope_in;
	*rec >> msg_in;

	string name;
	bool error = false;
	if (conID_in < MAX_CONNECT)
	{
		Player* p = gamePtr->getPlayer(conID_in);
		if (p == nullptr)
			error = true;
		else
			name = p->getName();
	}
	else
		name = "Server";

	if (error == false)
	{
		consolePtr->printMsg(msg_in, name, scope_in);
		if (scope_in == NET_MESSAGE::ALL)
			branch(rec, conID);
		if (scope_in == NET_MESSAGE::TEAM)
		{
			std::vector<int>* team = gamePtr->getTeamConIds(gamePtr->getPlayer(conID_in)->getTeam());
			for (int c = 0; c < team->size(); c++)
			{
				if (team->at(c) != conID)
					con[team->at(c)].send(rec);
			}
		}
	}
	else
		consolePtr->printMsg("ERROR in_message", "System", 'S');
}