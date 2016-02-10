#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "../Core/ui/UIManager.h"
#include "Connection.h"
#include "../Core/Game/Game.h"
#include "../Core/Console.h"

#include <iostream>
#include <glm/glm.hpp>
#include <string>

using namespace std;

#define MAX_CONNECT 20
#define PORT_DEFAULT 18180
#define tick 0.050f //Also change in Player.h

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
	UIManager* uiPtr;

	virtual void disconnected(Uint8 _conID) {};
public:
	
	//chat test
	string msg_out;
	Uint8 scope_out;
	//--------

	Topology() {};
	virtual void init(Console* console, UIManager* ui, int port, IpAddress addrs, bool record = false, bool playback = false) = 0;
	~Topology()
	{
		if (package)
			delete package;
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
	virtual void netlogUpdate(float dt) {};

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
		case NET_COMMAND::RESPAWN: in_command_respawn(rec, conID); break;
		case NET_COMMAND::ROLESWITCH: in_command_role_change(rec, conID); break;
		case NET_COMMAND::READY: in_command_gamemode_ready(rec, conID); break;
		}
	}

	void IN(Connection* connection, Uint8 conID)
	{
		while (true)
		{
			Packet* rec = new Packet();
			if (connection->receive(rec) == false && connection->isConnected())
			{	
				if(isClient)
					disconnected(0);
				else
					disconnected(conID);
			}

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
		temp->init(pName, glm::vec3(0, 0, 0), gamePtr->getPhysics());
		gamePtr->createPlayer(temp, p_conID, 100, ROLES::NROFROLES);
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

		//clear spectate ID if nessesary
		if (p_conID == gamePtr->spectateID)
			gamePtr->clearSpectateID();
	}

	virtual void event_gamemode_data() {};
	virtual void event_player_data() {};

	virtual void event_effect_timed_out(std::vector<EffectTimeOutInfo> alleffects) {};
	virtual void event_bullet_timed_out(std::vector<BulletTimeOutInfo> allbullets) {};
	virtual void event_bullet_hit_player(std::vector<BulletHitPlayerInfo> allhits) {};
	virtual void event_bullet_hit_world(std::vector<BulletHitWorldInfo> allhits) {};
	virtual void event_bullet_hit_effect(std::vector<BulletHitEffectInfo> allhits) {};
	virtual void event_effect_hit_player(std::vector<EffectHitPlayerInfo> allhits) {};

	virtual void in_event_respawn_denied(Packet* rec)
	{
		Uint8 tryAgain;
		*rec >> tryAgain;
		gamePtr->denyPlayerRespawn(tryAgain);
	}

	virtual void in_event_playerdata(Packet* rec)
	{
		Uint8 HP;
		for (int c = 0; c < 20; c++)
		{
			Player* p = gamePtr->getPlayer(c);
			if (p != nullptr)
			{
				*rec >> HP;
				p->setHP(HP);
			}
		}
	}

	virtual void in_event_gamemode_data(Packet* rec)
	{
		Uint8 type;
		*rec >> type;
		if (type == Uint8(GAMEMODE_TYPE::KOTH))
		{
			KingOfTheHill* koth = (KingOfTheHill*)gamePtr->getGameMode();
			Uint8 teamOneTokens;
			Uint8 teamTwoTokens;
			Uint8 teamOneOnCapP;
			Uint8 teamTwoOnCapP;
			Uint8 capPoint;
			Uint8 state;
			Uint8 lastMsg;
			*rec >> teamOneTokens >> teamTwoTokens >> teamOneOnCapP >> teamTwoOnCapP >> capPoint >> state >> lastMsg;
			koth->setGamemodeData(teamOneTokens, teamTwoTokens, teamOneOnCapP, teamTwoOnCapP, capPoint, (KOTHSTATE)state, (GAMEMODE_MSG)lastMsg);
		}
	}

	virtual void in_event_bullet_hit_player(Packet* rec)
	{
		BulletHitPlayerInfo hi;
		Uint8 playerHit, PID, BID, bt, hpTotal;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> playerHit >> PID >> BID >> bt >> hpTotal;
			hi.playerHit = playerHit;
			hi.bt = BULLET_TYPE(bt);
			hi.bulletBID = BID;
			hi.bulletPID = PID;
			hi.newHPtotal = hpTotal;
			gamePtr->handleBulletHitPlayerEvent(hi);
		}
	}

	virtual void in_event_bullet_hit_world(Packet* rec)
	{
		BulletHitWorldInfo hi;
		Uint8 PID, BID, bt;
		glm::vec3 hitpos; glm::vec3 hitdir; glm::vec4 cNorm;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> PID >> BID >> bt;
			*rec >> hitpos.x >> hitpos.y >> hitpos.z;
			*rec >> hitdir.x >> hitdir.y >> hitdir.z;
			*rec >> cNorm.x >> cNorm.y >> cNorm.z >> cNorm.w;
			hi.bt = BULLET_TYPE(bt);
			hi.bulletBID = BID;
			hi.bulletPID = PID;
			hi.hitPos = hitpos;
			hi.hitDir = hitdir;
			hi.collisionNormal = cNorm;
			gamePtr->handleBulletHitWorldEvent(hi);
		}
	}

	virtual void in_event_bullet_hit_effect(Packet* rec)
	{
		BulletHitEffectInfo hi;
		Uint8 PID, BID, bt;
		glm::vec3 hitpos; glm::vec3 hitdir; glm::vec4 cNorm;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> PID >> BID >> bt;
			*rec >> hitpos.x >> hitpos.y >> hitpos.z;
			*rec >> hitdir.x >> hitdir.y >> hitdir.z;
			*rec >> cNorm.x >> cNorm.y >> cNorm.z >> cNorm.w;
			hi.bt = BULLET_TYPE(bt);
			hi.bulletBID = BID;
			hi.bulletPID = PID;
			hi.hitPos = hitpos;
			hi.hitDir = hitdir;
			hi.collisionNormal = cNorm;
			gamePtr->handleBulletHitEffectEvent(hi);
		}
	}

	virtual void in_event_effect_hit_player(Packet* rec)
	{
		EffectHitPlayerInfo hi;
		Uint8 playerHit, PID, EID, et, hpTotal;
		glm::vec3 hitPosition;
		glm::vec3 pPosition;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> playerHit >> PID >> EID >> et;
			*rec >> hitPosition.x >> hitPosition.y >> hitPosition.z;
			*rec >> pPosition.x >> pPosition.y >> pPosition.z;
			*rec >> hpTotal;
			hi.playerHit = playerHit;
			hi.et = EFFECT_TYPE(et);
			hi.effectID = EID;
			hi.effectPID = PID;
			hi.newHPtotal = hpTotal;
			hi.hitPos = hitPosition;
			hi.playerPos = pPosition;
			gamePtr->handleEffectHitPlayerEvent(hi);
		}
	}

	virtual void in_event_bullet_time_out(Packet* rec)
	{
		BulletTimeOutInfo toi;
		Uint8 bt; Uint8 pid; Uint8 bid;
		glm::vec3 pos;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> bt >> pid >> bid;
			*rec >> pos.x >> pos.y >> pos.z;
			toi.bt = BULLET_TYPE(bt);
			toi.bulletPID = pid;
			toi.bulletBID = bid;
			toi.pos = pos;
			gamePtr->handleBulletTimeOuts(toi);
		}
	}

	virtual void in_event_effect_time_out(Packet* rec)
	{
		EffectTimeOutInfo toi;
		Uint8 bt; Uint8 pid; Uint8 bid;
		glm::vec3 pos;
		Uint8 size;
		*rec >> size;
		for (int c = 0; c < size; c++)
		{
			*rec >> bt >> pid >> bid;
			*rec >> pos.x >> pos.y >> pos.z;
			toi.et = EFFECT_TYPE(bt);
			toi.effectPID = pid;
			toi.effectID = bid;
			toi.pos = pos;
			gamePtr->handleEffectTimeOuts(toi);
		}
	}

	//Frame package FROM CLIENT
	virtual void frame_fire(WEAPON_TYPE wt, int conID, int bulletId, glm::vec3 pos, glm::vec3 dir, int team) 
	{ 
		*package << Uint8(NET_FRAME::FIRE) << Uint8(conID) << Uint8(team) << Uint8(bulletId) << Uint8(wt) <<
			pos.x << pos.y << pos.z <<
			dir.x << dir.y << dir.z;
	};

	virtual void frame_consumable(CONSUMABLE_TYPE ct, int conID, glm::vec3 pos, glm::vec3 dir, int team)
	{
		*package << Uint8(NET_FRAME::CONSUMABLE) << Uint8(conID) << Uint8(team) << Uint8(ct) <<
			pos.x << pos.y << pos.z <<
			dir.x << dir.y << dir.z;
	};

	virtual void frame_special_use(SPECIAL_TYPE st, int conID, int specialId, glm::vec3 pos, glm::vec3 dir, int team)
	{
		*package << Uint8(NET_FRAME::SPECIAL) << Uint8(conID) << Uint8(team) << Uint8(specialId) << Uint8(st) <<
			pos.x << pos.y << pos.z <<
			dir.x << dir.y << dir.z;
	}

	virtual void frame_weapon_switch(int conID, WEAPON_TYPE ws, int swaploc)
	{
		*package << Uint8(NET_FRAME::WPN_SWITCH) << Uint8(conID) << Uint8(ws) << Uint8(swaploc);
	}

	virtual void frame_name_change(Uint8 conid, string name)
	{
		*package << Uint8(NET_FRAME::NAME_CHANGE) << conid << name;
	}

	virtual void frame_pos(Uint8 conid, glm::vec3 cPos, glm::vec3 cDir, glm::vec3 cVel, Uint8 roomID)
	{
		*package << Uint8(NET_FRAME::POS) << conid <<
			cPos.x << cPos.y << cPos.z <<
			cDir.x << cDir.y << cDir.z <<
			cVel.x << cVel.y << cVel.z <<
			roomID;
	}

	virtual void frame_anim(Uint8 conid, Uint8 anim_peak_first, Uint8 anim_peak_third)
	{
		*package << Uint8(NET_FRAME::ANIM) << conid << anim_peak_first << anim_peak_third;
	}

	virtual void in_frame_fire(Packet* rec)
	{
		Uint8 conID;
		Uint8 team;
		Uint8 bulletId;
		Uint8 weapontype;
		glm::vec3 pos;
		glm::vec3 dir;
		*rec >> conID >> team >> bulletId >> weapontype;
		*rec >> pos.x >> pos.y >> pos.z;
		*rec >> dir.x >> dir.y >> dir.z;
		gamePtr->handleWeaponFire(conID, team, bulletId, WEAPON_TYPE(weapontype), pos, dir);
	}

	virtual void in_frame_consumable(Packet* rec)
	{
		Uint8 conID;
		Uint8 team;
		Uint8 consumabletype;
		glm::vec3 pos;
		glm::vec3 dir;
		*rec >> conID >> team >> consumabletype;
		*rec >> pos.x >> pos.y >> pos.z;
		*rec >> dir.x >> dir.y >> dir.z;
		gamePtr->handleConsumableUse(conID, team, CONSUMABLE_TYPE(consumabletype), pos, dir);
	}

	virtual void in_frame_special_use(Packet* rec)
	{
		Uint8 conID;
		Uint8 team;
		Uint8 sID;
		Uint8 specialtype;
		glm::vec3 pos;
		glm::vec3 dir;
		*rec >> conID >> team >> sID >> specialtype;
		*rec >> pos.x >> pos.y >> pos.z;
		*rec >> dir.x >> dir.y >> dir.z;
		gamePtr->handleSpecialAbilityUse(conID, team, sID, SPECIAL_TYPE(specialtype), pos, dir);
	}

	virtual void in_frame_weapon_switch(Packet* rec)
	{
		Uint8 p_conID;
		Uint8 swapLocation;
		Uint8 weapontype;
		*rec >> p_conID >> weapontype >> swapLocation;

		gamePtr->handleWeaponSwitch(p_conID, WEAPON_TYPE(weapontype), swapLocation);
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
		glm::vec3 p_vel;
		Uint8 p_roomid;
		*rec >> p_conID;
		*rec >> p_pos.x >> p_pos.y >> p_pos.z;
		*rec >> p_dir.x >> p_dir.y >> p_dir.z;
		*rec >> p_vel.x >> p_vel.y >> p_vel.z;
		*rec >> p_roomid;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p != nullptr) 
		{
			p->setGoalPos(p_pos);
			p->setGoalDir(p_dir);
			if (!p->isLocal())
				p->setVelocity(p_vel);
			p->roomID = p_roomid;
		}
		else
			consolePtr->printMsg("ERROR in_frame_current_pos", "System", 'S');
	}

	virtual void in_frame_anim(Packet* rec)
	{
		Uint8 p_conID, anim_peak_first, anim_peak_third;
		*rec >> p_conID;
		*rec >> anim_peak_first;
		*rec >> anim_peak_third;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p != nullptr)
		{
			p->setAnimState_f_p(AnimationState(anim_peak_first));
			p->setAnimState_t_p(AnimationState(anim_peak_third));
		}
	}

	//COMMANDS  From Client  --->  Server  - if ok -> Clients
	virtual void command_gamemode_ready(Uint8 conid)
	{
		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::READY) << conid << Uint8(0);
		con->send(out);
		delete out;
	}

	virtual void command_team_change(Uint8 conid, Uint8 team)
	{
		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::TEAM_CHANGE) << conid << team;
		con->send(out);
		delete out;
	}

	virtual void command_role_change(Uint8 conid, Uint8 role)
	{
		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::ROLESWITCH) << conid << role;
		con->send(out);
		delete out;
	}

	virtual void command_respawn(Uint8 conid)
	{
		Packet* out = new Packet();
		*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::RESPAWN) << conid << Uint8(0);
		con->send(out);
		delete out;
	}

	virtual void in_command_gamemode_ready(Packet* rec, Uint8 conID)
	{
		Uint8 p_conID;
		Uint8 ready;
		*rec >> p_conID >> ready;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p != nullptr)
		{
			if (p->getRole()->getRole() != ROLES::NROFROLES && p->getTeam() != 0)
			{
				if (!p->isReady())
				{
					if (ready == 0) //We are the server, here to register that someone's readying up
					{
						Packet* out = new Packet();
						*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::READY) << p_conID << Uint8(1);
						branch(out, -1);
						delete out;
					}
					gamePtr->getPlayer(p_conID)->setReady();
					consolePtr->printMsg(gamePtr->getPlayer(p_conID)->getName() + " is ready!", "System", 'S');
				}
			}	
		}
	}

	virtual void in_command_team_change(Packet* rec, Uint8 conID)
	{
		Uint8 p_conID;
		Uint8 team;
		*rec >> p_conID >> team;

		if (isClient)
		{
			if (team == 9)
			{
				consolePtr->printMsg("Can't join team!", "System", 'S');
				return;
			}
		}

		Player* p = gamePtr->getPlayer(p_conID);
		if (p == nullptr)
		{
			consolePtr->printMsg("ERROR in_command_team_change", "System", 'S');
			return;
		}

		if (isClient == false) //Decide if the command is OK
		{
			//if (gamePtr->getPlayersOnTeam(team) + 1 < gamePtr->getMaxTeamSize())
			//{
				/*
				TO DO
				Add logic to give player a new place to spawn after team change
				*/
			//}
			//else
			//{
				/*
				CAN'T CHANGE TEAM
				*/
			//}

			
			if (gamePtr->getPlayersOnTeam(team) < gamePtr->getMaxTeamSize())
			{

			}
			else //Too many people in team
			{
				Packet* out = new Packet();
				*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::TEAM_CHANGE) << p_conID << Uint8(9); //9 is a good error code, whatever
				con[p_conID].send(out);
				delete out;
				return;
			}
			Gamemode* gm = gamePtr->getGameMode();
			if (!gm->allowTeamChange()) //Check with the game mode
			{
				Packet* out = new Packet();
				*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::TEAM_CHANGE) << p_conID << Uint8(9);
				con[p_conID].send(out);
				delete out;
				return;
			}
		}

		if (team == 0)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team Spectators", "System", 'S');
		if (team == 1)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team One", "System", 'S');
		if (team == 2)
			consolePtr->printMsg("Player (" + p->getName() + ") joined team Two", "System", 'S');

		if (p_conID == getConId())
		{
			if (team == 0)
				gamePtr->freecam = true;
		}

		if (isClient == false)
		{
			Packet* out = new Packet();
			*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::TEAM_CHANGE) << p_conID << team;
			branch(out, -1);
			delete out;
		}
		//Adds player to the game in chosen team. Both clients and server
		gamePtr->addPlayerToTeam(p_conID, team);
		if (p_conID == getConId())
		{
			uiPtr->setFirstMenuSet(false);
			uiPtr->setMenu(2);
		}
	}

	virtual void in_command_role_change(Packet* rec, Uint8 conID)
	{
		Uint8 p_conID;
		Uint8 role;
		*rec >> p_conID >> role;

		if (isClient)
		{
			if (role == 9)
			{
				consolePtr->printMsg("Can't change role!", "System", 'S');
				return;
			}
		}

		Player* p = gamePtr->getPlayer(p_conID);
		if (p == nullptr)
		{
			consolePtr->printMsg("ERROR in_command_role_change", "System", 'S');
			return;
		}

		if (!isClient) //Check if we're allowed
		{
			Gamemode* gm = gamePtr->getGameMode();
			if (gm->allowRoleChange() || gamePtr->getPlayer(p_conID)->GetConsecutiveDeaths() > 4)
			{

			}
			else
			{
				Packet* out = new Packet();
				*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::ROLESWITCH) << p_conID << Uint8(9);
				con[p_conID].send(out);
				delete out;
				return;
			}
		}

		p->chooseRole(role - 1);
		if (p_conID == gamePtr->GetLocalPlayerId())
			gamePtr->setPlayerWantsToRespawn(true);
		gamePtr->sendPlayerRadSize(p->getRole()->getBoxRadius()); //TEMP BUT W/E
		consolePtr->printMsg("Player " + p->getName() + " switched class!", "System", 'S');

		if (isClient == false)
		{
			Packet* out = new Packet();
			*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::ROLESWITCH) << p_conID << role;
			branch(out, -1);
			delete out;
		}

		if (p_conID == getConId())
		{
			uiPtr->setOpenedGuiBool(true);
			uiPtr->setFirstMenuSet(false);
			uiPtr->setMenu(0);

			gamePtr->getPlayer(gamePtr->GetLocalPlayerId())->setLockedControls(false);
			gamePtr->setCursorInvisible(true);

			if (isClient)
			{
				Player* local = gamePtr->getPlayer(gamePtr->GetLocalPlayerId());
				KingOfTheHill* koth = (KingOfTheHill*)gamePtr->getGameMode();

				uiPtr->clearText(0);
				uiPtr->clearText(1);
				uiPtr->clearText(2);
				uiPtr->clearText(3);
				uiPtr->clearText(4);
				uiPtr->clearText(5);

				uiPtr->setText(std::to_string(local->getHP()), 0); //hp

				std::string nText = std::to_string(local->getAmmo()) + "/" + std::to_string(local->getMaxAmmo());
				uiPtr->setText(nText, 1); //ammo
				uiPtr->setText(std::to_string(koth->getRespawnTokens(1)), 2); //tickets
				uiPtr->setText(std::to_string(koth->getRespawnTokens(2)), 3); //tickets2
				uiPtr->setText(std::to_string(koth->getRoundWins(1)), 4); //wins1
				uiPtr->setText(std::to_string(koth->getRoundWins(2)), 5); //wins2
				if(int(koth->getTimer()) == 0)
					uiPtr->setText("00:00", 6); //time

				uiPtr->scaleBar(2, (float)(koth->getRespawnTokens(1)) / (float)(koth->getMaxTokensPerTeam()), false);
				uiPtr->scaleBar(3, (float)(koth->getRespawnTokens(2)) / (float)(koth->getMaxTokensPerTeam()), false);
				uiPtr->scaleBar(9, 1.0f, true);

				uiPtr->setRoleBool(true);
			}
		}
	}

	virtual void in_command_respawn(Packet* rec, Uint8 conid)
	{
		Uint8 p_conID;
		Uint8 respawnPosition; //if sent from client, this will be 0
		*rec >> p_conID >> respawnPosition;

		Player* p = gamePtr->getPlayer(p_conID);
		if (p == nullptr)
		{
			consolePtr->printMsg("ERROR in_command_respawn", "System", 'S');
			return;
		}

		if (isClient == false) //Decide if the command is OK
		{
			respawnPosition = gamePtr->findPlayerPosInTeam(conid) % 5; //Add logic to ask serverside game for respawn position for a given p_conID
			/*
			Authentication to make sure we actually can respawn - for now always Yes
			*/

			char tryAgain = 'N'; //Y/N
			bool playerCanRespawn = gamePtr->checkIfPlayerCanRespawn(conid, tryAgain);

			if (!playerCanRespawn)
			{
				Packet* out = new Packet();
			
				*out << Uint8(NET_INDEX::EVENT) << Uint8(NET_EVENT::RESPAWN_DENIED) << Uint8(tryAgain);
				con[p_conID].send(out);
				delete out;
				return;
			}
		}
		gamePtr->allowPlayerRespawn(p_conID, respawnPosition);

		if (isClient == false)
		{
			Packet* out = new Packet;
			*out << Uint8(NET_INDEX::COMMAND) << Uint8(NET_COMMAND::RESPAWN) << p_conID << respawnPosition;
			branch(out, -1);
			delete out;
		}
	}
};

#endif