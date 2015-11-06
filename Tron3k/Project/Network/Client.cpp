#include "Client.h"

Client::~Client()
{

}

void Client::init()
{
	isClient = true;

	nrCon = 1;

	con = new Connection();
	con->init();
}

