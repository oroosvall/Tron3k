#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "Connection.h"

#include <iostream>
#include <string>

using namespace std;


#define MAX_CONNECT 20


class Topology
{
private:

protected:
	int PORT = 18180;
	Connection* con = 0;
	bool isClient;

public:
	Topology() {};
	virtual void init() = 0;
	~Topology()
	{
		if (con)
			delete[] con;
	}
	virtual bool bind() { return false; };
	virtual void update(float dt) = 0;
	virtual void network_IN(float dt) = 0;
	virtual void network_OUT(float dt) = 0;
	virtual bool new_connection() = 0;

	void in_ping(Packet* rec, Uint8 conID) {};
	virtual void in_new_connection(Packet* rec, Uint8 conID) = 0;
	virtual void in_event(Packet* rec, Uint8 conID) = 0;
	virtual void in_frame(Packet* rec, Uint8 conID) = 0;

	void IN(Connection* connection, Uint8 conID)
	{
		while (true)
		{
			Packet* rec = new Packet();
			connection->receive(rec);
			if (rec->getDataSize() == 0) // no more messages to handle
				break;

			//extract net index
			Uint8 index_short;
			*rec >> index_short;
			NET_INDEX index = NET_INDEX(index_short);

			switch (index)
			{
			case PING:				in_ping(rec, conID); break;
			case NEW_CONNECTION:	in_new_connection(rec, conID); break;
			case EVENT:				in_event(rec, conID); break;
			case FRAME:				in_frame(rec, conID); break;
			default:
				//package error?
				break;
			}
			delete rec;
		}
	}

};

#endif