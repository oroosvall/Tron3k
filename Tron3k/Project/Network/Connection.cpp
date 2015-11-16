#include "Connection.h"

Connection::~Connection()
{
	
}

void Connection::init()
{
	socket.setBlocking(false);
	connected = false;
}

bool Connection::connect(IpAddress _address, Int32 _port)
{
	socket.setBlocking(true);
	status = socket.connect(_address, _port, seconds(2));
	socket.setBlocking(false);
	if (status != sf::Socket::Done)
	{
		connected = false;
		return connected;
	}

	connected = true;
	return connected;
}

bool Connection::connect(TcpListener* listener)
{
	if (listener->accept(socket) != sf::Socket::Done)
	{
		connected = false;
		return connected;
	}

	connected = true;
	return connected;
}

bool Connection::receive(Packet* ret)
{
	status = socket.receive(*ret);
	if (status != sf::Socket::Done)
		return false;

	return true;
}

bool Connection::send(Packet* out)
{
	status = socket.send(*out);
	if (status != sf::Socket::Done)
		return false;

	return true;
}