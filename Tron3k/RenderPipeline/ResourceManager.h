#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>

#define KB 1024
#define MB KB*KB

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
