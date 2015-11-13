#include "SimpleShit.h"

struct TriangleVertex
{
	float x, y, z;
	float u, v;
};

struct Index
{
	unsigned int f1, f2, f3;
};

void TestMesh::make()
{
	loadVert(string("testBoat.v"));
	loadBMP(string("boat.bmp"));

	//bind
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);


	//glGenBuffers(1, &vbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	//
	//glBufferDataARB(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW_ARB);
	//
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//
	//glGenBuffers(1, &index);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	//
	//glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexstr), &indexstr, GL_STATIC_DRAW_ARB);
	//
}

bool TestMesh::loadVert(string path)
{
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		std::vector<TriangleVertex> vert;
		std::vector<TriangleVertex> uv;
		std::vector<GLushort> Indices;

		std::string sub;
		int count = 0;
		int state = 0;
		while (true) {
			if (!(getline(myfile, line))) break;
			if (line.size() < 5)
				continue;
			if (line[0] == 'v' && line[1] == ' ') // vertex pos
			{
				vert.push_back(TriangleVertex());

				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'v'
				iss >> sub;
				vert[count].x = std::stof(sub);
				iss >> sub;
				vert[count].y = std::stof(sub);
				iss >> sub;
				vert[count].z = std::stof(sub);
				vert[count].u = -1.0f;
				vert[count].v = -1.0f;
				count++;
			}
			else if (line[0] == 'v' && line[1] == 't') //UV cord
			{
				if (state != 1)
				{
					state = 1;
					count = 0;
				}
				uv.push_back(TriangleVertex());
				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'vt'
				iss >> sub;
				uv[count].u = std::stof(sub);
				iss >> sub;
				uv[count].v = std::stof(sub);
				count++;
			}
			else if (line[0] == 'f') // face
			{
				if (state != 2)
				{
					state = 2;
					count = 0;
				}
				std::istringstream iss(line);
				std::string sub;
				Indices.push_back(0);
				Indices.push_back(0);
				Indices.push_back(0);
				//vert
				iss >> sub; // discard 'f'

				for (int n = 0; n < 3; n++)
				{
					std::string pos;
					iss >> pos; // vertex index
					iss >> sub; // uv index
					int indexVERT = std::stoi(pos) - 1;
					int indexUV = std::stoi(sub) - 1;
					if (vert[indexVERT].u < 0)
					{
						Indices[count * 3 + n] = indexVERT; // set vertex index
						vert[indexVERT].u = uv[indexUV].u;
						vert[indexVERT].v = uv[indexUV].v;
					}
					else
					{
						int temp = indexVERT;
						indexVERT = vert.size();
						vert.push_back(TriangleVertex());
						vert[indexVERT] = vert[temp];
						vert[indexVERT].u = uv[indexUV].u;
						vert[indexVERT].v = uv[indexUV].v;
						Indices[count * 3 + n] = indexVERT;
					}
					iss >> sub; // normal index
				}
				count++;
			}
		}

		glGenBuffers(1, &vbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert[0])* vert.size(), &vert[0], GL_STATIC_DRAW);

		glGenBuffers(1, &index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * count * 3, &Indices[0], GL_STATIC_DRAW);
		faceCount = count;
		glEnableVertexAttribArray(index);

		//define vertex data layout
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float) * 3));
		return true;
	}
	else //file not found
		return false;
}

bool TestMesh::loadBMP(string imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
							  // Actual RGB data
	unsigned char * data;

	FILE* file = fopen(imagepath.c_str(), "rb");
	if (!file)
		return false;

	if (fread(header, 1, 54, file) != 54) // If not 54 bytes read : problem
		return false;

	if (header[0] != 'B' || header[1] != 'M')
		return false;

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way

					  // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	glGenTextures(1, &textureId);

	glActiveTexture(GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	delete[] data;

	return true;
}