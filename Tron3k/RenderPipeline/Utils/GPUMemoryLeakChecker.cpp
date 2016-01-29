#define REDEFCALLS

#include "GPUMemoryLeakChecker.h"
#include <iostream>
#include <Windows.h>

#include <map>

using std::cout;
using std::endl;

vector<GLBuffer> genBufferChecks;
vector<GLBuffer> genVertexArrayChecks;
vector<GLBuffer> genTextureCheck;

unsigned int drawCount = 0;
unsigned int primitiveCount = 0;

unsigned int genBufferPeak = 0;
unsigned int genVaoPeak = 0;
unsigned int genTexturePeak = 0;

unsigned int memusage = 0;

unsigned int textureBinds = 0;
unsigned int bufferBinds = 0;

unsigned int shaderBinds = 0;

GLuint lastProgram = 0;
GLenum currentActiveTexture = GL_TEXTURE0;

GLenum arrayBufferLast = 0;
GLenum elementBufferLast = 0;

GLuint textureBindMap[5];


void glGenBuffers_D(GLsizei n, GLuint* id, char* file, int line)
{
	GLBuffer buf;
	
	glGenBuffers(n, id);

	buf.bufferID = *id;
	buf.file = std::string(file);
	buf.line = line;
	buf.free = false;

	genBufferChecks.push_back(buf);
	genBufferPeak++;
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
	genVaoPeak++;
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
	genTexturePeak++;
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
	genBufferPeak--;
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
	genVaoPeak--;
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
	genTexturePeak--;
}

void glDrawElements_D(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	drawCount++;
	glDrawElements(mode, count, type, indices);

	if(mode == GL_TRIANGLES)
		primitiveCount += (count / 3);
}

void glDrawArrays_D(GLenum mode, GLint first, GLsizei count)
{
	drawCount++;
	glDrawArrays(mode, first, count);
	if (mode == GL_TRIANGLES)
		primitiveCount += (count / 3);
	else if(mode == GL_TRIANGLE_STRIP)
		primitiveCount += count;

}

void glBufferData_D(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
{
	GLint64 oldBufferSize = 0;
	glGetBufferParameteri64v(target, GL_BUFFER_SIZE, &oldBufferSize);
	glBufferData(target, size, data, usage);

	memusage += (size - oldBufferSize);
}

void glActiveTexture_D(GLenum texture)
{
	currentActiveTexture = texture;
	glActiveTexture(texture);
}

void glBindTexture_D(GLenum target, GLuint texture)
{
	if (textureBindMap[currentActiveTexture - GL_TEXTURE0] != texture)
	{
		textureBinds++;
		glBindTexture(target, texture);
		textureBindMap[currentActiveTexture - GL_TEXTURE0] = texture;
	}
}

void glBindBuffer_D(GLenum target, GLuint buffer)
{
	glBindBuffer(target, buffer);
}

void glTexImage2D_D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data)
{
	int oldW = 0, oldH = 0;
	int oldSize = 0;

	int newW = 0, newH = 0;
	int newSize = 0;

	int compressed = 0;

	int internalComponentSize = 0;

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &oldSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		oldSize = oldW * oldH * internalComponentSize;

	}
	glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &newSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &newW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &newH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		newSize = newW * newH * internalComponentSize;

	}

	memusage += (newSize - oldSize);

}

void glCompressedTexImage2D_D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data)
{

	int oldW = 0, oldH = 0;
	int oldSize = 0;

	int newW = 0, newH = 0;
	int newSize = 0;

	int compressed = 0;

	int internalComponentSize = 0;

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &oldSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &oldW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &oldH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		oldSize = oldW * oldH * internalComponentSize;

	}

	glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	memusage += imageSize;

	glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED, (GLint*)&compressed);
	if (compressed)
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &newSize);
	else
	{
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &newW);
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &newH);

		glGetTexLevelParameteriv(target, level, GL_TEXTURE_RED_SIZE, &internalComponentSize);
		int temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_GREEN_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_BLUE_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_ALPHA_SIZE, &temp);
		internalComponentSize += temp;
		glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_SIZE, &temp);
		internalComponentSize += temp;

		internalComponentSize /= 8;

		newSize = newW * newH * internalComponentSize;

	}

	memusage += (newSize - oldSize);

}

void glUseProgram_D(GLuint program)
{
	if (program != lastProgram)
	{
		shaderBinds++;
		glUseProgram(program);
	}
	lastProgram = program;
}

void reportGPULeaks()
{
	bool found = false;
	for (size_t i = 0; i < genBufferChecks.size(); i++)
	{
		if (genBufferChecks[i].free == false)
		{
			cout << "glGenBuffers: " << genBufferChecks[i].bufferID << "\n" << genBufferChecks[i].file << " : " << genBufferChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genVertexArrayChecks.size(); i++)
	{
		if (genVertexArrayChecks[i].free == false)
		{
			cout << "glGenVertexArrays: " << genVertexArrayChecks[i].bufferID << "\n" << genVertexArrayChecks[i].file << " : " << genVertexArrayChecks[i].line << endl;
			found = true;
		}
	}

	for (size_t i = 0; i < genTextureCheck.size(); i++)
	{
		if (genTextureCheck[i].free == false)
		{
			cout << "glGenTextures: " << genTextureCheck[i].bufferID << "\n" << genTextureCheck[i].file << " : " << genTextureCheck[i].line << endl;
			found = true;
		}
	}

	if (found)
	{
		//breakLaptopBySlammingItClosedCauseIWasWatchingPornAtWork();
		// if break here u have gpu mem leaks, press break and check console
		DebugBreak();
	}

}

#undef glGenBuffers
#undef glGenVertexArrays
#undef glGenTextures

#undef glDeleteBuffers
#undef glDeleteVertexArrays
#undef glDeleteTextures

#undef glDrawElements
#undef glDrawArrays_D

#undef glBufferData

#undef glActiveTexture
#undef glBindTexture

#undef glBindBuffer

#undef glTexImage2D
#undef glCompressedTexImage2D

#undef glUseProgram

#define glGenBuffers(n,i)			glGenBuffers_D(n,i, __FILE__, __LINE__)
#define glGenVertexArrays(n,i)		glGenVertexArray_D(n,i, __FILE__, __LINE__)
#define glGenTextures(n,i)			glGenTexture_D(n,i, __FILE__, __LINE__)

#define glDeleteBuffers(n,i)		glDeleteBuffers_D(n,i)
#define glDeleteVertexArrays(n,i)	glDeleteVertexArray_D(n,i)
#define glDeleteTextures(n,i)		glDeleteTexture_D(n,i)

#define glDrawElements(m, c, t ,i)		glDrawElements_D(m, c, t ,i)

#define glDrawArrays(m, f, c)		glDrawArrays_D(m, f, c)

#define glBufferData(target, size, data, usage) glBufferData_D(target, size, data, usage)

#define glActiveTexture(texture) glActiveTexture_D(texture)
#define glBindTexture(target, texture) glBindTexture_D(target, texture)

#define glBindBuffer(target, buffer)	glBindBuffer_D(target, buffer) 

#define glTexImage2D(target, level, internalFormat, width, height, border, format, type, data) glTexImage2D_D(target, level, internalFormat, width, height, border, format, type, data)

#define glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data) glCompressedTexImage2D_D(target, level, internalformat, width, height, border, imageSize, data)


#define glUseProgram(program)	glUseProgram_D(program)
