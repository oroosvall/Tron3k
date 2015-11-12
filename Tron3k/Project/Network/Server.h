#ifndef SERVER_H
#define SERVER_H

#include "Topology.h"

class Server : public Topology
{
private:
	TcpListener listener;
	int nrConnected = 0;
	int nrConnectedRot = 0;

public:
	void init();
	~Server();

	//Core->Topology com
	void network_IN(float dt);
	void network_OUT(float dt);
	bool new_connection();
	int getConId() { return 'S'; };

	//server only
	virtual bool bind();
	virtual void branch(Packet* rec, Uint8 conID);

	//package
	void package_clear();

	//in
	void in_new_connection(Packet* rec, Uint8 conID);
	void in_event(Packet* rec, Uint8 conID);
	void in_frame(Packet* rec, Uint8 conID);
	void in_message(Packet* rec, Uint8 conID);

};

#endif