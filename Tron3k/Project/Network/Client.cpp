#include "Client.h"

void Client::disconnected(Uint8 _conID)
{
	consolePtr->printMsg("Server Disconnected", "System", 'S');
	con[_conID].disconnect();
}

Client::~Client()
{
	if (con)
		delete con;
	if (package)
		delete package;
}

void Client::init(Console* console, UIManager* ui, int port, IpAddress addrs, bool record, bool playback)
{
	address = addrs;
	PORT = port;
	consolePtr = console;
	uiPtr = ui;

	package = new Packet();
	*package << Uint8(NET_INDEX::FRAME);

	isClient = true;

	firstPackageSent = false;
	packageRecieved = false;
	mapLoaded = false;
	joined = false;

	conID = -1;

	con = new Connection();
	//test recording
	con->init(record, playback);
}

bool Client::network_IN(float dt)
{
	IN(con, conID);
	if (con->isConnected())
		return true;
	return false;
}

void Client::network_OUT(float dt)
{
	if (msg_out != "")
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::MESSAGE) << conID << scope_out << msg_out;
			con->send(out);
		delete out;

		msg_out = "";
	}

	if(package->getDataSize() > 1) //only send if contains data
		con->send(package);
	package_clear();
}

bool Client::new_connection()
{
	// test connect
	if (con->isConnected() == false)
	{
		if (con->connect(address, PORT))
		{
			return true;
		}
		return false; //failed
	}
	return true; //??? already connected
}

void Client::new_connection_packet(string _name)
{
	Packet* out;
	out = new Packet();
	*out << Uint8(NET_INDEX::NEW_CONNECTION);
	*out << _name;
	con->send(out);
	delete out;
}

void Client::netlogUpdate(float dt)
{
	con->netLogUpdate(dt);
}

void Client::package_clear()
{
	if (package)
		delete package;
	package = new Packet();
	*package << Uint8(NET_INDEX::FRAME);
}

void Client::in_new_connection(Packet* rec, Uint8 _conID)
{
	packageRecieved = true;

	string map;
	*rec >> conID;
	*rec >> map;

	Uint8 pExists;
	string pName;
	Uint8 team;
	Uint8 role;
	Uint8 hp;
	Player* temp;
	for (int c = 0; c < MAX_CONNECT; c++)
	{
		*rec >> pExists;
		if (pExists == 1)
		{
			temp = new Player();
			*rec >> pName;
			temp->init(pName, glm::vec3(0, 0, 0), gamePtr->getPhysics());
			*rec >> team;
			temp->setTeam(team);
			*rec >> role;
			*rec >> hp;
			gamePtr->createPlayer(temp, c, hp, role);
			delete temp;
			gamePtr->addPlayerToTeam(c, team);
		}
	}

	/*
	COOLDOWNS FOR PICKUPS
	*/
	std::vector<Effect*> effects = gamePtr->getEffects(EFFECT_TYPE::HSCPICKUP);
	HSCPickup* hsc = nullptr;
	float cd = 0.0f;
	for (int c = 0; c < effects.size(); c++)
	{
		*rec >> cd;
		hsc = (HSCPickup*)effects[c];
		hsc->setCooldown(cd);
	}
	
	effects = gamePtr->getEffects(EFFECT_TYPE::DOUBLEDAMAGEPICKUP);
	DoubleDamagePickup* dd = nullptr;
	for (int c = 0; c < effects.size(); c++)
	{
		*rec >> cd;
		dd = (DoubleDamagePickup*)effects[c];
		dd->setCooldown(cd);
	}


	temp = new Player();
	temp->init("ClientName", glm::vec3(0, 0, 0), gamePtr->getPhysics());
	gamePtr->createPlayer(temp, conID, 100, ROLES::NROFROLES, true);
	delete temp;
	
	printf("My connection ID : %d \n", conID);
	//can i load this map?
	printf("Map in use : %s \n", map.c_str());

}

void Client::in_event(Packet* rec, Uint8 _conID)
{
	Uint8 event_type;
	while (!rec->endOfPacket())
	{
		*rec >> event_type;
		switch (event_type)
		{
		case NET_EVENT::PLAYER_JOINED:	in_event_player_joined(rec); break;
		case NET_EVENT::PLAYER_LEFT: in_event_player_left(rec); break;
		case NET_EVENT::GAMEMODE_DATA: in_event_gamemode_data(rec); break;
		case NET_EVENT::PLAYERDATA: in_event_playerdata(rec); break;
		case NET_EVENT::PLAYER_HIT: in_event_bullet_hit_player(rec); break;
		case NET_EVENT::BULLET_WORLD_HIT: in_event_bullet_hit_world(rec); break;
		case NET_EVENT::BULLET_EFFECT_HIT: in_event_bullet_hit_effect(rec); break;
		case NET_EVENT::EFFECT_PLAYER_HIT: in_event_effect_hit_player(rec); break;
		case NET_EVENT::BULLET_TIMEOUT: in_event_bullet_time_out(rec); break;
		case NET_EVENT::EFFECT_TIMEOUT: in_event_effect_time_out(rec); break;
		case NET_EVENT::RESPAWN_DENIED: in_event_respawn_denied(rec); break;
		}
	}
}

void Client::in_frame(Packet* rec, Uint8 conID)
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
}

void Client::in_message(Packet* rec, Uint8 conID)
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
		consolePtr->printMsg(msg_in, name, scope_in);
	else
		consolePtr->printMsg("ERROR in_message", "System", 'S');
}