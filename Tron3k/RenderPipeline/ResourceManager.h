#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>

class ResourceManager
{
private:

	unsigned int memoryLimit;
	unsigned int memoryUsed;

	void readConfig();

public:
	
	ResourceManager();
	std::string getStatus();



};

#endif
