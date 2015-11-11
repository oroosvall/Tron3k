#ifndef CLIENT_H
#define CLIENT_H

#include "Topology.h"

class Client : public Topology
{
private:
	Uint8 conID;
	bool firstPackageSent;
	bool packageRecieved;
	bool mapLoaded;
	bool joined;
public:
	~Client();
	void init();

	void update(float dt);
	void network_IN(float dt);
	void network_OUT(float dt);
	bool new_connection();

	void in_new_connection(Packet* rec, Uint8 conID);
	void in_event(Packet* rec, Uint8 conID);
	void in_frame(Packet* rec, Uint8 conID);

	virtual bool firstPackageRecieved() { return packageRecieved; };
};

#endif