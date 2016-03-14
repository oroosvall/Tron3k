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

			printf("LOADER: Loading texture %s from streaming thread\n", it->second.c_str());

			void* textureData = loadTextureData(it->second, format, x, y, size);

			printf("LOADER: Loading texture done\n");

			StreamedData data;
			data.x = x;
			data.y = y;
			data.format = format;
			data.size = size;
			data.fileName = it->second;

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

			StreamedData s = it->second;

			printf("UPLOAD: Streaming texture %d\n", *it->first);
			printf("UPLOAD: Texture name %s\n", s.fileName.c_str());

			texturePos = *it->first;

			dataQueue.erase(it);
			dataQueueEmpty = dataQueue.empty();

			glGenTextures(1, &texID);

			if (!(s.format == GL_COMPRESSED_RGBA || s.format == GL_RGBA))
			{
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
				glCompressedTexImage2D(GL_TEXTURE_2D, 0, s.format,s.x, s.y, 0, s.size, s.data);

			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, s.x, s.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, s.data);


			}

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 100.0f);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0.0f);
			//glGenerateMipmap(GL_TEXTURE_2D);

			delete[](char*)s.data;
			s.data = nullptr;

			dataQMutex.unlock();

		}
		unsigned int t2 = clock();
		printf("UPLOAD: streaming time %d\n\n", t2 - t1);

	}

	return texID;

}