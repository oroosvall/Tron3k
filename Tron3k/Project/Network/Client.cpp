#include "Client.h"

Client::~Client()
{
	
}

void Client::init()
{
	isClient = true;

	firstPackageSent = false;
	firstPackageRecieved = false;
	mapLoaded = false;
	joined = false;

	con = new Connection();
	con->init();
}

void Client::update(float dt)
{
	network_IN(dt);

	network_OUT(dt);
}

void Client::network_IN(float dt)
{
	if (con->isConnected() == false)
		new_connection();
	if (firstPackageSent == true)
		IN(con, Uint8(0));
}

void Client::network_OUT(float dt)
{
	if (firstPackageSent == false)
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::NEW_CONNECTION);
		con->send(out); //can return false!!!
		delete out;

		firstPackageSent = true;
	}
	else if (firstPackageRecieved)
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
	{
		for (int n = 0; n < 5; n++)
		{
			if (con->connect(IpAddress::LocalHost, PORT))
			{
				printf("Connected to server\n");
				return true;
			}
			printf("Connection failed...");
		}

		return false;
	}
	return true; //??? already connected
}

void Client::in_new_connection(Packet* rec, Uint8 _conID)
{
	firstPackageRecieved = true;

	Uint8 index_short;
	//conid already clasmember
	string map;
	*rec >> index_short;
	*rec >> conID;
	*rec >> map;

	cout << "My connection ID :" << conID << endl;
	//can i load this map?
	cout << "Map in use : " << map << endl;

}

void Client::in_event(Packet* rec, Uint8 _conID)
{

}

void Client::in_frame(Packet* rec, Uint8 conID)
{

}