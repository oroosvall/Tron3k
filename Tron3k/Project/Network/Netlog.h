#ifndef NETLOG_H
#define NETLOG_H

#include <sfml\Network.hpp>
#include <fstream>
#include <string>
#include <sstream>

using sf::Packet;
using namespace std;

class Netlog
{
private:
	float timepass;

	string fileName;
	fstream file;
	bool open;
	Packet current;
	float current_timestamp;
	bool newLoaded;

	void initBase(string filename);
public:
	~Netlog();
	void initREC(string filename);
	void initPLAY(string filename);
	void update(float dt);
	void log(Packet* rec);
	void read(Packet* ret);
};

#endif
