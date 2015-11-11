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

}

void Client::network_IN(float dt)
{
	IN(con, conID);
}

void Client::network_OUT(float dt)
{
	delta += dt;

	if (delta > 0.100)
	{
		if (msg_out != "")
		{
			Packet* out;
			out = new Packet();
			*out << Uint8(NET_INDEX::MESSAGE) << conID << scope_out << msg_out;
				con->send(out);
			delete out;

			msg_out = "";
			delta = 0;
		}
	}
}

bool Client::new_connection()
{
	// test connect
	if (con->isConnected() == false)
	{
		if (con->connect(IpAddress("192.168.1.67"), PORT))
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
	con->send(out);
	delete out;
}

void Client::in_new_connection(Packet* rec, Uint8 _conID)
{
	packageRecieved = true;

	string map;
	*rec >> conID;
	Uint32 size;
	*rec >> map;
	
	printf("My connection ID : %d \n", conID);
	//can i load this map?
	printf("Map in use : %s \n", map.c_str());

}

void Client::in_event(Packet* rec, Uint8 _conID)
{

}

void Client::in_frame(Packet* rec, Uint8 conID)
{

}

void Client::in_message(Packet* rec, Uint8 conID)
{
	Uint8 _conid; //from what client
	*rec >> _conid;
	*rec >> scope_in;
	*rec >> msg_in;

	msg_in = to_string(_conid) + " > " + msg_in;
}