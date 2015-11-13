#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "Connection.h"
#include "../Core/Game/Game.h"
#include "../Core/Console.h"

#include <iostream>
#include <glm/glm.hpp>
#include <string>

using namespace std;

#define MAX_CONNECT 20
#define PORT_DEFAULT 18180
#define tick 0.050f

class Topology
{
private:

protected:
	int PORT = PORT_DEFAULT;
	Connection* con = 0;
	bool isClient;

	Packet* package;

	Game* gamePtr;
	Console* consolePtr;
public:

	//chat test
	string msg_out;
	Uint8 scope_out;
	//--------

	Topology() {};
	virtual void init(Console* console) = 0;
	~Topology()
	{
		if (con)
		{
			if (isClient)
				delete con;
			else
				delete[] con;
		}
	}

	//Core->Topology com
	virtual void network_IN(float dt) = 0;
	virtual void network_OUT(float dt) = 0;
	virtual bool new_connection() = 0;
	virtual Uint8 getConId() = 0;
	virtual void setPort(int port) { PORT = port; };
	virtual void setPortDefault() { PORT = PORT_DEFAULT; };
	virtual void setGamePtr(Game*& ptr) { gamePtr = ptr; };
	virtual bool is_client() { return isClient; };

	//client only
	virtual void setIP(IpAddress addr) { };
	virtual bool firstPackageRecieved() { return false; };
	virtual void new_connection_packet() {};

	//server only
	virtual bool bind() { return false; };
	virtual void branch(Packet* rec, Uint8 condID) {};

	//in
	void in_ping(Packet* rec, Uint8 conID) {};
	virtual void in_new_connection(Packet* rec, Uint8 conID) = 0;
	virtual void in_event(Packet* rec, Uint8 conID) = 0;
	virtual void in_frame(Packet* rec, Uint8 conID) = 0;
	virtual void in_message(Packet* rec, Uint8 conID) = 0;

	void IN(Connection* connection, Uint8 conID)
	{
		while (true)
		{
			Packet* rec = new Packet();
			connection->receive(rec);
			if (rec->getDataSize() == 0) // no more messages to handle
			{
				delete rec;
				break;
			}

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
			case MESSAGE:			in_message(rec, conID); break;
			default:
				//package error?
				break;
			}
			delete rec;
		}
	}

	//package
	virtual void package_clear() = 0;

	//Event package FROM SERVER

	//Frame package FROM CLIENT
	virtual void frame_pos() { };
	virtual void frame_jump() { };
	virtual void frame_fire() { };

	virtual void frame_name_change(Uint8 conid, string name)
	{
		*package << Uint8(NAME_CHANGE) << conid << name;
	}

	virtual void in_frame_name_change(Packet* rec)
	{
		Uint8 p_conID;
		string pNewName;
		*rec >> p_conID >> pNewName;
		Player* p = gamePtr->getPlayer(p_conID);
		if (p != nullptr)
		{
			consolePtr->printMsg("Player (" + p->getName() + ") changed name to (" + pNewName + ")", "System", 'S');
			p->setName(pNewName);
		}
		else
			consolePtr->printMsg("ERROR in_frame_name", "System", 'S');
	}
};

#endif