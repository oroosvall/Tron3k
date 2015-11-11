#include "Client.h"

Client::~Client()
{
	
}

void Client::init()
{
	isClient = true;

	firstPackageSent = false;
	packageRecieved = false;
	mapLoaded = false;
	joined = false;

	conID = -1;

	con = new Connection();
	con->init();
}

void Client::update(float dt)
{
	//network_IN(dt);

	//network_OUT(dt);
}

void Client::network_IN(float dt)
{
	IN(con, conID);
}

void Client::network_OUT(float dt)
{
	if (packageRecieved == false)
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::NEW_CONNECTION);
		con->send(out); //can return false!!!
		delete out;

		firstPackageSent = true;
	}
	else if (packageRecieved)
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::FRAME) << conID;
		con->send(out);
		delete out;
	}
}

bool Client::new_connection()
{
	// test connect
	if (con->isConnected() == false)
		if (con->connect(IpAddress::LocalHost, PORT))
			return true;
		return false; //failed
	return true; //??? already connected
}

void Client::in_new_connection(Packet* rec, Uint8 _conID)
{
	packageRecieved = true;

	Uint8 index_short;
	//conid already clasmember
	string map;
	*rec >> index_short;
	*rec >> conID;
	*rec >> map;

	printf("My connection ID : d% \n", conID);
	//can i load this map?
	printf("Map in use : s% \n", map);

}

void Client::in_event(Packet* rec, Uint8 _conID)
{

}

void Client::in_frame(Packet* rec, Uint8 conID)
{

}