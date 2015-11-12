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
	void init();
	~Client();
	
	//Core->Topology com
	void network_IN(float dt);
	void network_OUT(float dt);
	bool new_connection();
	int getConId() { return conID; };

	//client only
	virtual void setIP(IpAddress addr) { address = addr; };
	virtual bool firstPackageRecieved() { return packageRecieved; };
	virtual void new_connection_packet();

	//package
	void package_clear();

	//in
	void in_new_connection(Packet* rec, Uint8 conID);
	void in_event(Packet* rec, Uint8 conID);
	void in_frame(Packet* rec, Uint8 conID);
	void in_message(Packet* rec, Uint8 conID);
	
};

#endif