#include "Server.h"

Server::~Server()
{
	
}

void Server::init()
{
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

void Server::bounce(Packet* rec, Uint8 conID)
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

void Server::update(float dt)
{
	network_IN(dt);

	network_OUT(dt);
}

void Server::network_IN(float dt)
{
	//look for new connections
	new_connection(); //shoudlnt do this every frame

	//fetch packages from all clients
	for (int n = 0; n < MAX_CONNECT; n++)
		IN(&con[n], n);

}

void Server::network_OUT(float dt)
{

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
	Uint8 scope;
	*rec >> conID;
	*rec >> scope;
	*rec >> msg_in;

	printf("%d > ", conID);
	printf("%s\n", msg_in.c_str());

	if(scope == NET_MESSAGE::ALL)
		bounce(rec, conID);
}