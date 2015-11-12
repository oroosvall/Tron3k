#include "Server.h"

Server::~Server()
{
	
}

void Server::init()
{
	multipacket = 0;

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

void Server::network_IN(float dt)
{
	//fetch packages from all clients
	for (int n = 0; n < MAX_CONNECT; n++)
		IN(&con[n], n);

}

void Server::network_OUT(float dt)
{
	if (msg_out != "")
	{
		Packet* out;
		out = new Packet();
		*out << Uint8(NET_INDEX::MESSAGE) << Uint8('S') << scope_out << msg_out;
		con->send(out);
		delete out;

		msg_out = "";
	}
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
			}
		}
	}

	return true;  //isnt used when on server
}

void Server::in_new_connection(Packet* rec, Uint8 conID)
{
	Uint8 index_short;
	*rec >> index_short;
	Packet* out = new Packet();

	//type of NET_ID
	*out << Uint8(NET_INDEX::NEW_CONNECTION);

	//the player's id
	*out << conID;

	//mapname used
	*out << string("tempMap");

	//reply to the connection
	con[conID].send(out);
	delete out;

	cout << "sent new connection data to" << to_string(conID) << endl;
}

void Server::in_event(Packet* rec, Uint8 conID)
{

}

void Server::in_frame(Packet* rec, Uint8 conID)
{

}

void Server::in_message(Packet* rec, Uint8 conID)
{
	*rec >> conID_in;
	*rec >> scope_in;
	*rec >> msg_in;

	if(scope_in == NET_MESSAGE::ALL)
		branch(rec, conID);
}