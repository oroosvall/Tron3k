#ifndef CONNECTION_H
#define CONNECTION_H

#include <SFML\Network.hpp>
#include "NetIndex.h"

using namespace sf;

#define TIMEOUT 2000

class Connection
{
private:
	TcpSocket socket;
	sf::Socket::Status status;
	bool connected;

public:
	void init();
	bool connect(IpAddress _address, Int32 _port);
	bool connect(TcpListener* listener);
	bool receive(Packet* ret);
	bool send(Packet* out);

	bool isConnected() { return connected; };
	void setConnected(bool set) { connected = set; };

	TcpSocket* getSocket() { return &socket; };
};

#endif