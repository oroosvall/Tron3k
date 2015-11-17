#include "Client.h"

Client::~Client()
{
	if (con)
		delete con;
	if (package)
		delete package;
}

void Client::init(Console* console, int port, IpAddress addrs)
{
	address = addrs;
	PORT = port;
	consolePtr = console;

	package = new Packet();
	*package << Uint8(NET_INDEX::FRAME);

	isClient = true;

	firstPackageSent = false;
	packageRecieved = false;
	mapLoaded = false;
	joined = false;

	conID = -1;

	con = new Connection();
	con->init();
}

void Client::network_IN(float dt)
{
	IN(con, conID);
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

void Client::new_connection_packet()
{
	Packet* out;
	out = new Packet();
	*out << Uint8(NET_INDEX::NEW_CONNECTION);
	*out << "ClientName";
	con->send(out);
	delete out;
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
	Player* temp;
	for (int c = 0; c < MAX_CONNECT; c++)
	{
		*rec >> pExists;
		if (pExists == 1)
		{
			temp = new Player();
			*rec >> pName;
			temp->init(pName, glm::vec3(0, 0, 0));
			gamePtr->createPlayer(temp, c);
			delete temp;
		}
	}

	temp = new Player();
	temp->init("ClientName", glm::vec3(0, 0, 0));
	gamePtr->createPlayer(temp, conID, true);
	delete temp;
	
	printf("My connection ID : %d \n", conID);
	//can i load this map?
	printf("Map in use : %s \n", map.c_str());

}

void Client::in_event(Packet* rec, Uint8 _conID)
{
	Uint8 event_type;
	*rec >> event_type;
	switch (event_type)
	{
	case NET_EVENT::PLAYER_JOINED:
		Player* temp = new Player;
		Uint8 p_conID;
		string pName;
		*rec >> p_conID;
		*rec >> pName;
		temp->init(pName, glm::vec3(0, 0, 0));
		gamePtr->createPlayer(temp, p_conID);
		consolePtr->printMsg("Player (" + pName + ") joined the server", "System", 'S');
		delete temp;
		break;
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
		case NET_FRAME::POS: in_frame_pos(rec); break;
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