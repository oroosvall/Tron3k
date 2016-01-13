#include "Netlog.h"

Netlog::~Netlog()
{
	if (file.is_open())
		file.close();
}

void Netlog::initBase(string filename)
{
	fileName = "GameFiles/Netlog/" + filename;
	open = false;
	newLoaded = false;
	timepass = 0;
}

void Netlog::initREC(string filename)
{
	initBase(filename);
	file.open(fileName, ios::out | ios::trunc | ios::binary);
}

void Netlog::initPLAY(string filename)
{
	initBase(filename);
	file.open(fileName, ios::in | ios::binary);
}

void Netlog::update(float dt)
{
	timepass += dt;
}

void Netlog::log(Packet* rec)
{
	unsigned int datasize = rec->getDataSize();
	if (datasize == 0) //dont write !!
		return;

	char* data;
	data = (char*)rec->getData();
	unsigned int time = timepass * 1000.0f;
	file.write((char*)&time, 4);
	file.write((char*)&datasize, 4);
	file.write(data, datasize);
}

void Netlog::read(Packet* ret)
{
	//if packet is ready to be read
	if (timepass + 1.0f > current_timestamp)
	{
		*ret = current;

		if (file.eof() == false) //if not at end of file
		{
			unsigned int time = 0;
			unsigned int datasize = 0;
			file.read((char*)&time, 4);
			file.read((char*)&datasize, 4);
			
			if (datasize != 0) 
			{
				char* data = new char[datasize];
				file.read(data, datasize);

				current_timestamp = ((float)time) / 1000.0f;
				current = Packet();
				current.append(data, datasize);

				delete[] data;
			}
			else //end of file
			{
				timepass = -10000000000.0f;
			}
		}
		else // end of file
		{
			timepass = -10000000000.0f;
		}
	}
}