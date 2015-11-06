#include "Server.h"

Server::~Server()
{

}

void Server::init()
{
	isClient = false;

	nrCon = MAX_CONNECT;

	con = new Connection[nrCon];

	for (int n = 0; n < nrCon; n++)
		con[n].init();
}

void Server::network_IN(float dt)
{

}