#ifndef CLIENT_H
#define CLIENT_H

#include "Topology.h"

class Client : public Topology
{
private:

public:
	~Client();
	void init();
	void update(float dt);

	void network_IN(float dt);
	void network_OUT(float dt);
};

#endif