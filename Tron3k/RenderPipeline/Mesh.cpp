#include "Mesh.h"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

void Mesh::init(float x, float y, float z)
{
	world[0].w = x;
	world[1].w = y;
	world[2].w = z;
}

void Mesh::release()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}

Mesh::~Mesh()
{

}

void Mesh::load(std::string path)
{

	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		std::vector<Vertex> vert;
		std::vector<Vertex> normal;
		std::vector<Vertex> uv;
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
				vert.push_back(Vertex());

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
				uv.push_back(Vertex());
				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'vt'
				iss >> sub;
				uv[count].u = std::stof(sub);
				iss >> sub;
				uv[count].v = std::stof(sub);
				count++;
			}
			else if (line[0] == 'v' && line[1] == 'n')  // normals
			{
				if (state != 2)
				{
					state = 2;
					count = 0;
				}
				normal.push_back(Vertex());
				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'vn'
				iss >> sub;
				normal[count].n1 = std::stof(sub);
				iss >> sub;
				normal[count].n2 = std::stof(sub);
				iss >> sub;
				normal[count].n3 = std::stof(sub);
				normal[count].u = -1.0f;
				normal[count].v = -1.0f;
				count++;
			}
			else if (line[0] == 'f') // face
			{
				if (state != 3)
				{
					state = 3;
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
					std::string norm;
					iss >> pos; // vertex index
					iss >> sub; // uv index
					iss >> norm; // normal index
					int indexVERT = std::stoi(pos) - 1;
					int indexUV = std::stoi(sub) - 1;
					int indexNORM = std::stoi(norm) - 1;
					if (vert[indexVERT].u < 0)
					{
						Indices[count * 3 + n] = indexVERT; // set vertex index
						vert[indexVERT].u = uv[indexUV].u;
						vert[indexVERT].v = uv[indexUV].v;

						vert[indexVERT].n1 = normal[indexNORM].n1;
						vert[indexVERT].n2 = normal[indexNORM].n2;
						vert[indexVERT].n3 = normal[indexNORM].n3;

					}
					else
					{
						int temp = indexVERT;
						indexVERT = vert.size();
						vert.push_back(Vertex());
						vert[indexVERT] = vert[temp];
						vert[indexVERT].u = uv[indexUV].u;
						vert[indexVERT].v = uv[indexUV].v;

						vert[indexVERT].n1 = normal[indexNORM].n1;
						vert[indexVERT].n2 = normal[indexNORM].n2;
						vert[indexVERT].n3 = normal[indexNORM].n3;

						Indices[count * 3 + n] = indexVERT;
					}

					
				}
				count++;
			}
		}

	
		//setMaxAndMinPos(vert);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert[0])* vert.size(), &vert[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * count * 3, &Indices[0], GL_STATIC_DRAW);
		faceCount = count;
		//glEnableVertexAttribArray(index);

		//define vertex data layout
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
		//uv
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 6));
		// normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(float) * 3));


	}

}

void Mesh::setTexture(GLuint texID)
{
	textureID = texID;
}

float* Mesh::getWorld()
{
	return &world[0][0];
}

std::vector<float> Mesh::getBoundaries()
{
	std::vector<float> boundaries;

	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = -FLT_MAX;

	minX = -100.0f;
	maxX = 100.0f;
	minY = -15.0f;
	maxY = 1.0f;
	minZ = -100.0f;
	maxZ = 100.0f;

	boundaries.push_back(maxX);
	boundaries.push_back(minX);
	boundaries.push_back(maxY);
	boundaries.push_back(minY);
	boundaries.push_back(maxZ);
	boundaries.push_back(minZ);

	return boundaries;
}