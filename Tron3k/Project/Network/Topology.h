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

	virtual void disconnected(Uint8 _conID) {};
public:
	
	//chat test
	string msg_out;
	Uint8 scope_out;
	//--------

	Topology() {};
	virtual void init(Console* console, int port, IpAddress addrs) = 0;
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
	virtual bool network_IN(float dt) = 0;
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
	virtual void new_connection_packet(string name) {};

	//server only
	virtual bool bind() { return false; };
	virtual void branch(Packet* rec, Uint8 condID) {};

	//in
	void in_ping(Packet* rec, Uint8 conID) {};
	virtual void in_new_connection(Packet* rec, Uint8 conID) = 0;
	virtual void in_event(Packet* rec, Uint8 conID) = 0;
	virtual void in_frame(Packet* rec, Uint8 conID) = 0;
	virtual void in_message(Packet* rec, Uint8 conID) = 0;
	virtual void in_command(Packet* rec, Uint8 conID)
	{
		Uint8 command_type;
		*rec >> command_type;
		switch (command_type)
		{
		case NET_COMMAND::TEAM_CHANGE: in_command_team_change(rec, conID); break;
		}
	}

	void IN(Connection* connection, Uint8 conID)
	{
		while (true)
		{
			Packet* rec = new Packet();
			if(connection->receive(rec) == false && connection->isConnected())
				disconnected(conID);

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
			case COMMAND:			in_command(rec, conID); break;
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
	virtual void in_event_player_joined(Packet* rec)
	{
		Player* temp = new Player;
		Uint8 p_conID;
		string pName;
		*rec >> p_conID;
		*rec >> pName;
		temp->init(pName, glm::vec3(0, 0, 0));
		gamePtr->createPlayer(temp, p_conID);
		consolePtr->printMsg("Player (" + pName + ") joined the server", "System", 'S');
		delete temp;
	}

	virtual void in_event_player_left(Packet* rec)
	{
		Uint8 p_conID;
		*rec >> p_conID;
		Player* temp;
		temp = gamePtr->getPlayer(p_conID);
		if (temp != nullptr)
		{
			consolePtr->printMsg("Player (" + temp->getName() + ") Left the server", "System", 'S');
			gamePtr->removePlayer(p_conID);
		}
		else
			consolePtr->printMsg("ERROR in_event_player_left" , "System", 'S');
	}

	//Frame package FROM CLIENT
	virtual void frame_fire(WEAPON_TYPE wt, int teamId, glm::vec3 pos, glm::vec3 dir) 
	{ 
		*package << Uint8(NET_FRAME::FIRE) << Uint8(teamId) << Uint8(wt) << 
			pos.x << pos.y << pos.z <<
			dir.x << dir.y << dir.z;
	};

	virtual void frame_name_change(Uint8 conid, string name)
	{
		*package << Uint8(NET_FRAME::NAME_CHANGE) << conid << name;
	}

	virtual void frame_pos(Uint8 conid, glm::vec3 cPos, glm::vec3 cDir)
	{
		*package << Uint8(NET_FRAME::POS) << conid << cPos.x << cPos.y << cPos.z << cDir.x << cDir.y << cDir.z;
	}

	virtual void in_frame_fire(Packet* rec)
	{
		Uint8 team;
		Uint8 weapontype;
		glm::vec3 pos;
		glm::vec3 dir;
		*rec >> team >> weapontype;
		*rec >> pos.x >> pos.y >> pos.z;
		*rec >> dir.x >> dir.y >> dir.z;
		gamePtr->handleWeaponFire(team, WEAPON_TYPE(weapontype), pos, dir);
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

	virtual void in_frame_pos(Packet* rec)
	{
		Uint8 p_conID;
		glm::vec3 p_pos;
		glm::vec3 p_dir;
		*rec >> p_conID;
		*rec >> p_pos.x >> p_pos.y >> p_pos.z;
		*rec >> p_dir.x >> p_dir.y >> p_dir.z;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p != nullptr) //Justincase
		{
			//TO DO: Player function to interpolate for 50ms to new position
			p->setGoalPos(p_pos);
			p->setGoalDir(p_dir);
		}
		else
			consolePtr->printMsg("ERROR in_frame_current_pos", "System", 'S');
	}

	//COMMANDS  From Client  --->  Server  - if ok -> Clients
	virtual void command_team_change(Uint8 conid, Uint8 team)
	{
		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::TEAM_CHANGE) << conid << team;
		con->send(out);
		delete out;
	}

	virtual void in_command_team_change(Packet* rec, Uint8 conID)
	{
		Uint8 p_conID;
		Uint8 team;
		*rec >> p_conID >> team;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p == nullptr)
		{
			consolePtr->printMsg("ERROR in_command_team_change", "System", 'S');
			return;
		}

		if (isClient == false) //Decide if the command is OK
		{
			if (gamePtr->getPlayersOnTeam(team) + 1 < gamePtr->getMaxTeamSize())
			{
				Packet* out; out = new Packet();
				string reply = "Teamchange Accepted";
				*out << Uint8(NET_INDEX::MESSAGE) << Uint8(getConId()) << Uint8('S') << reply;
				con[p_conID].send(out);
				delete out;
			}
			else
			{
				Packet* out; out = new Packet();
				string reply = "Teamchange denied";
				*out << Uint8(NET_INDEX::MESSAGE) << Uint8(getConId()) << Uint8('S') << reply;
				con[p_conID].send(out);
				delete out;
				return;
			}
		}

		gamePtr->addPlayerToTeam(p_conID, team);
		if (team == 0)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team Spectators", "System", 'S');
		if (team == 1)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team One", "System", 'S');
		if (team == 2)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team Two", "System", 'S');
		

		if (isClient == false)
			branch(rec, -1);
	}
};

#endif