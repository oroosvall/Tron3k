#include "Server.h"

void Server::disconnected(Uint8 _conID)
{
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

void Server::event_bullet_hit_player(BulletHitPlayerInfo hi, int newHPtotal)
{
	Packet* out = new Packet();
	*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::PLAYER_HIT);
	*out << Uint8(hi.playerHit) << Uint8(hi.bulletPID) << Uint8(hi.bulletBID) << Uint8(hi.bt);
	*out << Uint8(newHPtotal);
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

void Server::init(Console* console, int port, IpAddress addrs)
{
	PORT = port;
	consolePtr = console;

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
		}
		else
		{
			*out << Uint8(0);
			if (c == conID)
			{
				Player* temp = new Player();
				temp->init(pName, glm::vec3(0, 0, 0));
				gamePtr->createPlayer(temp, conID);
				delete temp;
			}
		}
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
		case NET_FRAME::POS: in_frame_pos(rec); break;
		case NET_FRAME::FIRE: in_frame_fire(rec); break;
		case NET_FRAME::WPN_SWITCH: in_frame_weapon_switch(rec); break;
		case NET_FRAME::SPECIAL: in_frame_special_use(rec); break;
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
	}
	else
		consolePtr->printMsg("ERROR in_message", "System", 'S');
}

