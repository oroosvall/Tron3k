#include "Mesh.h"

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
				vert[count].n1 = -1.0f;
				vert[count].n2 = -1.0f;
				vert[count].n3 = -1.0f;

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
			else if (line[0] == 'v' && line[1] == 'n') //UV cord
			{
				if (state != 1)
				{
					state = 1;
					count = 0;
				}
				normal.push_back(Vertex());
				std::istringstream iss(line);
				std::string sub;
				iss >> sub; // discard 'vn'
				iss >> sub;
				vert[count].n1 = std::stof(sub);
				iss >> sub;
				vert[count].n2 = std::stof(sub);
				iss >> sub;
				vert[count].n3 = std::stof(sub);
				vert[count].u = -1.0f;
				vert[count].v = -1.0f;
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
					}
					else
					{
						int temp = indexVERT;
						indexVERT = vert.size();
						vert.push_back(Vertex());
						vert[indexVERT] = vert[temp];
						vert[indexVERT].u = uv[indexUV].u;
						vert[indexVERT].v = uv[indexUV].v;
						Indices[count * 3 + n] = indexVERT;
					}
					
				}
				count++;
			}
		}

}