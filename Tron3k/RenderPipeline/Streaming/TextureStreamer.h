#ifndef TEXTURESTREAMER_H
#define TEXTURESTREAMER_H

#include <map>
#include "../Utils/GPUMemoryLeakChecker.h"
#include <mutex>

struct StreamedData
{
	int x;
	int y;
	int format;
	int size;

	std::string fileName;

	void* data;
};

extern bool stremingThreadRunning;
extern std::map<GLuint *, std::string> streamingQueue;
extern std::mutex streamingQMutex;

extern std::map<GLuint *, StreamedData> dataQueue;
extern std::mutex dataQMutex;

extern bool dataQueueEmpty;


void streamingThread();

bool addToStreamQueue(GLuint *, std::string);

GLuint uploadStreamedData(unsigned int &id);

#endif