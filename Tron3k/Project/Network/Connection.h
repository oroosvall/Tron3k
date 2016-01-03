#ifndef CONNECTION_H
#define CONNECTION_H

#include <SFML\Network.hpp>
#include "NetIndex.h"
#include "Netlog.h"

using namespace sf;

#define TIMEOUT 2000

class Connection
{
private:
	TcpSocket socket;
	sf::Socket::Status status;
	bool connected;

	Netlog netlog;
	bool recording;
	bool playback;

public:
	~Connection();
	void init(bool recording = false, bool playback = false);
	bool connect(IpAddress _address, Int32 _port);
	bool connect(TcpListener* listener);
	bool receive(Packet* ret);
	bool send(Packet* out);
	void disconnect();
	void netLogUpdate(float dt);
	bool isConnected() { return connected; };
	void setConnected(bool set) { connected = set; };
	TcpSocket* getSocket() { return &socket; };
};

#endif