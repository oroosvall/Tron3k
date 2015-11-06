#ifndef SERVER_H
#define SERVER_H

#include "Topology.h"

class Server : public Topology
{
private:

public:
	~Server();
	void init();
	void update(float dt);
	
	void network_IN(float dt);
	void network_OUT(float dt);
};

#endif