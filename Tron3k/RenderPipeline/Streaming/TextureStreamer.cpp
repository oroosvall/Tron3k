#include "TextureStreamer.h"

#include "../Texture.h"

#include <time.h>

bool stremingThreadRunning;

std::map<GLuint* , std::string> streamingQueue;
std::mutex streamingQMutex;

std::map<GLuint *, StreamedData> dataQueue;
std::mutex dataQMutex;


bool dataQueueEmpty = true;

void streamingThread()
{
	stremingThreadRunning = true;

	while (stremingThreadRunning)
	{
		if(!streamingQueue.empty())
		{
			streamingQMutex.lock();
			std::map<GLuint* , std::string>::iterator it = streamingQueue.begin();

			int x = 0;
			int y = 0;
			int size = 0;
			int format = 0;

			printf("Loading texture %s from streaming thread\n", it->second.c_str());

			void* textureData = loadTextureData(it->second, format, x, y, size);

			printf("Loading texture done\n");

			StreamedData data;
			data.x = x;
			data.y = y;
			data.format = format;
			data.size = size;

			data.data = textureData;

			dataQMutex.lock();
			dataQueue[it->first] = data;
			dataQMutex.unlock();

			streamingQueue.erase(it);
			dataQueueEmpty = false;

			streamingQMutex.unlock();

		}
		else
		{
			Sleep(200);
		}
	}

}


bool addToStreamQueue(GLuint* textureIDPtr, std::string path)
{
	
	if (streamingQMutex.try_lock())
	{
		streamingQueue[textureIDPtr] = path;
		printf("Added %d, %s to queue\n", (int)textureIDPtr, path.c_str());
		streamingQMutex.unlock();
		return true;
	}

	return false;
}

GLuint uploadStreamedData(unsigned int &texturePos)
{
	GLuint texID = 0;

	if (!dataQueueEmpty)
	{

		unsigned int t1 = clock();
		if (dataQMutex.try_lock())
		{
			std::map<GLuint *, StreamedData>::iterator it = dataQueue.begin();

			printf("Streaming texture %d\n", *it->first);

			texturePos = *it->first;

			glGenTextures(1, &texID);

			if (!(it->second.format == GL_COMPRESSED_RGBA || it->second.format == GL_RGBA))
			{
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
				glCompressedTexImage2D(GL_TEXTURE_2D, 0, it->second.format, it->second.x, it->second.y, 0, it->second.size, it->second.data);

			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, it->second.x, it->second.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, it->second.data);


			}

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

			delete[](char*)it->second.data;

			dataQueue.erase(it);
			dataQueueEmpty = dataQueue.empty();

			dataQMutex.unlock();

		}
		unsigned int t2 = clock();
		printf("streaming time %d\n", t2 - t1);

	}

	return texID;

}