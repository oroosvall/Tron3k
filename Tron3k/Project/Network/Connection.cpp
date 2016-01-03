#include "Connection.h"

Connection::~Connection()
{
	
}

void Connection::init(bool rec, bool play)
{
	socket.setBlocking(false);
	connected = false;

	recording = rec;
	playback = play;

	if (recording)
	{
		netlog.initREC("test.bin");
		playback = false; // both cant be true
	}
	else if (playback)
		netlog.initPLAY("test.bin");
}

bool Connection::connect(IpAddress _address, Int32 _port)
{
	if (playback == false)
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
	else
	{
		connected = true;
		return connected;
	}
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

void Connection::disconnect()
{
	socket.disconnect();
	connected = false;
}

bool Connection::receive(Packet* ret)
{
	if (playback == false)
	{
		status = socket.receive(*ret);
		if (status == sf::Socket::Disconnected)
			return false;

		if (recording)
			netlog.log(ret);

		return true;
	}
	else
	{
		netlog.read(ret);
		return true;
	}
}

bool Connection::send(Packet* out)
{
	status = socket.send(*out);
	if (status != sf::Socket::Done)
		return false;

	return true;
}

void Connection::netLogUpdate(float dt)
{
	netlog.update(dt);
}
