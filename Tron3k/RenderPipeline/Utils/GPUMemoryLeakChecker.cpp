#define REDEFCALLS

#include "GPUMemoryLeakChecker.h"
#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;

vector<GLBuffer> genBufferChecks;
vector<GLBuffer> genVertexArrayChecks;
vector<GLBuffer> genTextureCheck;

void glGenBuffers_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;
	
	glGenBuffers(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genBufferChecks.push_back(buf);
}

void glGenVertexArray_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenVertexArrays(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;
	
	genVertexArrayChecks.push_back(buf);
}

void glGenTexture_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;

	glGenTextures(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genTextureCheck.push_back(buf);
}

void glDeleteBuffers_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].bufferID == *id)
		{
			glDeleteBuffers(n, id);
			genBufferChecks.erase(genBufferChecks.begin()+i);
			break;
		}
	}
}

void glDeleteVertexArray_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].bufferID == *id)
		{
			glDeleteVertexArrays(n, id);
			genVertexArrayChecks.erase(genVertexArrayChecks.begin() + i);
			break;
		}
	}
}

void glDeleteTexture_D(GLsizei n, GLuint* id)
{
	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].bufferID == *id)
		{
			glDeleteTextures(n, id);
			genTextureCheck.erase(genTextureCheck.begin() + i);
			break;
		}
	}
}

void reportGPULeaks()
{
	bool found = false;
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].free == false)
		{
			cout << "glGenBuffers\n" << genBufferChecks[i].file << " : " << genBufferChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].free == false)
		{
			cout << "glGenVertexArrays\n" << genVertexArrayChecks[i].file << " : " << genVertexArrayChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].free == false)
		{
			cout << "glGenTextures\n" << genTextureCheck[i].file << " : " << genTextureCheck[i].line << endl;
			found = true;
		}
	}

	if (found)
	{
		DebugBreak();
	}

}

#undef glGenBuffers
#undef glGenVertexArray
#undef glGenTexture

#undef glDeleteBuffers
#undef glDeleteVertexArray
#undef glDeleteTexture

#define glGenBuffers(n,i)			glGenBuffers_D(n,i, __FILE__, __LINE__)
#define glGenVertexArray(n,i)		glGenVertexArray_D(n,i, __FILE__, __LINE__)
#define glGenTexture(n,i)			glGenTexture_D(n,i, __FILE__, __LINE__)

#define glDeleteBuffers(n,i)		glDeleteBuffers_D(n,i)
#define glDeleteVertexArray(n,i)	glDeleteVertexArray(n,i)
#define glDeleteTexture(n,i)		glDeleteTexture_D(n,i)