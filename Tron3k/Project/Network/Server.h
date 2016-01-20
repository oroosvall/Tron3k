#ifndef SERVER_H
#define SERVER_H

#include "Topology.h"

class Server : public Topology
{
private:
	TcpListener listener;
	int nrConnected = 0;
	int nrConnectedRot = 0;

	void disconnected(Uint8 _conID);
public:
	void init(Console* console, int port, IpAddress addrs, bool record = false, bool playback = false);
	~Server();

	//Core->Topology com
	bool network_IN(float dt);
	void network_OUT(float dt);
	bool new_connection();
	Uint8 getConId() { return 'S'; };

	//server only
	bool bind();
	void branch(Packet* rec, Uint8 conID);

	//package
	void package_clear();

	//in
	void in_new_connection(Packet* rec, Uint8 conID);
	void in_event(Packet* rec, Uint8 conID);
	void in_frame(Packet* rec, Uint8 conID);
	void in_message(Packet* rec, Uint8 conID);

	void event_bullet_hit_player(std::vector<BulletHitPlayerInfo> allhits);

	void event_bullet_hit_world(std::vector<BulletHitWorldInfo> allhits);
	void event_effect_hit_player(std::vector<EffectHitPlayerInfo> allhits);
};

#endif