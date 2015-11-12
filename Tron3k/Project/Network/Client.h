#ifndef CLIENT_H
#define CLIENT_H

#include "Topology.h"

class Client : public Topology
{
private:
	IpAddress address;
	Uint8 conID;
	bool firstPackageSent;
	bool packageRecieved;
	bool mapLoaded;
	bool joined;
public:
	~Client();
	void init();

	void network_IN(float dt);
	void network_OUT(float dt);
	bool new_connection();
	

	void in_new_connection(Packet* rec, Uint8 conID);
	void in_event(Packet* rec, Uint8 conID);
	void in_frame(Packet* rec, Uint8 conID);
	void in_message(Packet* rec, Uint8 conID);

	virtual bool firstPackageRecieved() { return packageRecieved; };
	virtual void new_connection_packet();
	virtual void setIP(IpAddress addr) { address = addr; };
};

#endif