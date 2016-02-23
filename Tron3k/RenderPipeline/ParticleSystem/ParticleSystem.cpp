#include "ParticleSystem.h"

void ParticleSystem::Initialize(glm::vec3 pos, ParticleSystemData ps, GLuint* program, ParticleShaderLocations* pLoc)
{
	m_program = program;
	m_data = ps;

	m_loc = pLoc;

	m_size = glm::vec2(m_data.width, m_data.height);

	m_vertices = new Particle[m_data.maxparticles];

	for (int i = 0; i < m_data.maxparticles; i++)
	{
		Particle p;
		//p.pos = glm::vec4(0, 0, 0, 0);
		p.pos = glm::vec4(pos, 0);
		glm::vec3 dir = m_data.dir;
		if (m_data.omni)
		{
			float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f));
			dir = glm::normalize(glm::vec3(x-1.0f, y - 1.0f, z - 1.0f));
		}
		p.iDir = glm::vec4(dir, 0);
		p.dir = glm::vec4(dir, m_data.lifetime);
		//m_vertices.push_back(p);
		m_vertices[i] = p;
	}

	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_data.maxparticles, &m_vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
	//pos
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(0));
	//dir
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(sizeof(float) * 4));
	//iDir
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 3, BUFFER_OFFSET(sizeof(float) * 8));

	//glProgramUniform1f(*m_program, m_loc->lifetime, m_data->lifetime);
	//glProgramUniform1f(*m_program, m_loc->force, m_data->force);
	//glProgramUniform1f(*m_program, m_loc->drag, m_data->drag);
	//glProgramUniform1f(*m_program, m_loc->gravity, m_data->gravity);
	//glProgramUniform1f(*m_program, m_loc->emission, m_data->emission);
	//glProgramUniform1i(*m_program, m_loc->continuous, m_data->continuous);
	//glProgramUniform1i(*m_program, m_loc->omni, m_data->omni);
	//glProgramUniform3f(*m_program, m_loc->initialPos, m_pos.x, m_pos.y, m_pos.z);
	
	m_pos = pos;

	m_counter = 0;
	m_alive = false;
}


void ParticleSystem::Update(float dT)
{
	int emit = 123;
	if (m_currentEmission < 0.0f)
	{
		m_currentEmission = m_data.emission;
		emit = -1;

		if (!m_data.continuous)
		{
			m_counter++;
		}
	}
	else
	{
		m_currentEmission -= dT;
	}

	if (m_data.emission == 0.0f)
	{
		emit = 0;
		if (m_counter != m_data.maxparticles)
		{
			m_counter = m_data.maxparticles;
			m_currentEmission = m_data.lifetime;
		}
	}

	if (m_counter == m_data.maxparticles && m_currentEmission < 0.0f)
	{
		m_alive = true;
	}

	glProgramUniform1f(*m_program, m_loc->lifetime, m_data.lifetime);
	glProgramUniform1f(*m_program, m_loc->force, m_data.force);
	glProgramUniform1f(*m_program, m_loc->drag, m_data.drag);
	glProgramUniform1f(*m_program, m_loc->gravity, m_data.gravity);
	glProgramUniform1i(*m_program, m_loc->continuous, m_data.continuous);
	glProgramUniform1i(*m_program, m_loc->omni, m_data.omni);
	glProgramUniform3f(*m_program, m_loc->initialPos, m_pos.x, m_pos.y, m_pos.z);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo);

	glProgramUniform1f(*m_program, m_loc->deltaTime, dT);
	glProgramUniform1i(*m_program, m_loc->emission, emit);

	glDispatchCompute(1, 1, 1);
}

void ParticleSystem::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindVertexArray(m_vao);

	glDrawArrays(GL_POINTS, 0, m_data.maxparticles);
}

void ParticleSystem::Release()
{
	delete[] m_vertices;
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}