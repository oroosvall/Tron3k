#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "../Core/Core.h"
#include "../Engine/Engine.h"
#include "Connection.h";

#define MAX_CONNECT 20

class Topology
{
private:

protected:
	Core* core = 0;
	Engine* engine = 0;

	Connection* con = 0;
	int nrCon = 0;

	bool isClient;

public:
	~Topology()
	{
		if (core)
			delete core;
		if (engine)
			delete engine;
		if (con)
			delete[] con;
	}

	virtual void init() = 0;
	void update(float dt)
	{
		network_IN(dt);

		core->update(dt);

		network_OUT(dt);

		engine->reneder(core);
	}
	virtual void network_IN(float dt) = 0;
	virtual void network_OUT(float dt) = 0;

};

#endif