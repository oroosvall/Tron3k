#include "ParticleSystem.h"
#include <Windows.h>

void ParticleSystem::Initialize(glm::vec3 pos, ParticleSystemData ps, GLuint* program, ParticleShaderLocations* pLoc)
{
	m_vao = 0;
	m_vbo = 0;
	m_program = program;
	m_data = ps;

	m_loc = pLoc;

	m_size = glm::vec2(m_data.width, m_data.height);

	m_vertices = new Particle[m_data.maxparticles];

	std::random_device rd;
	std::mt19937 mt(m_data.seed);
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	for (int i = 0; i < m_data.maxparticles; i++)
	{
		Particle p;
		//p.pos = glm::vec4(0, 0, 0, 0);
		p.pos = glm::vec4(pos, 0);
		glm::vec3 dir = m_data.dir;
		//if (m_data.omni)
		{
			float x = dist(mt);
			float y = dist(mt);
			float z = dist(mt);
			dir = glm::vec3(x, y, z);
		}
		p.iDir = glm::vec4(dir, 0);
		if(m_data.emission == 0.0f)
			p.dir = glm::vec4(dir, m_data.lifetime);
		else
		{
			p.dir = glm::vec4(dir, -1.0f);
		}
		//m_vertices.push_back(p);
		m_vertices[i] = p;
	}

	created = false;

	if (m_data.maxparticles)
	{
		glGenBuffers(1, &m_vbo);
		glGenVertexArrays(1, &m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//int asd;
		//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &asd);
		//if(asd != m_vbo)	// while current bound array buffer is not equal to m_vbo do this loop, "Should never happen"
		//{
		//	// something failed so delete the buffer and vao, created remains false so no update will occur
		//	// set m_alive to true to delete object, best confuse ever!
		//	glDeleteBuffers(1, &m_vbo);
		//	glDeleteVertexArrays(1, &m_vao);
		//
		//	m_vbo = 0;
		//	m_vao = 0;
		//
		//	m_alive = true;
		//	return;
		//}
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
		m_glow = m_data.glow;
		m_scaleDir = m_data.scaleDir;
		created = true;
	}

}


void ParticleSystem::Update(float dT)
{
	if (created)
	{

		int emit = 0;
		if (m_currentEmission < 0.0f)
		{
			m_currentEmission += m_data.emission;
			emit = 1;
			if (!m_data.continuous)
			{
				if (m_data.emission)
					m_counter++;
			}
		}
		else
		{
			m_currentEmission -= dT;
		}

		if (m_data.emission == 0.0f)
		{
			if (m_counter != m_data.maxparticles)
			{
				emit = m_data.maxparticles;
				m_counter = m_data.maxparticles;
				m_currentEmission = m_data.lifetime;
			}
		}

		timer += dT;

		if (m_counter >= m_data.maxparticles && m_currentEmission < 0.0f)
		{
			if(timer > (m_data.lifetime + m_data.emission* m_data.maxparticles))
				m_alive = true;
		}

		// force removal
		if (timer > 100.0f)
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
		glProgramUniform1f(*m_program, m_loc->spread, m_data.spread);
		glProgramUniform3f(*m_program, m_loc->sysDir, m_data.dir.x, m_data.dir.y, m_data.dir.z);


		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo);

		glProgramUniform1f(*m_program, m_loc->deltaTime, dT);
		glProgramUniform1i(*m_program, m_loc->emission, emit);

		glDispatchCompute(1, 1, 1);
	}
}

void ParticleSystem::Draw()
{
	if (created)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindVertexArray(m_vao);

		glDrawArrays(GL_POINTS, 0, m_data.maxparticles);
	}
}

void ParticleSystem::Release()
{
	delete[] m_vertices;

	if (created)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

		m_vao = 0;
		m_vbo = 0;
	}
}

void ParticleSystem::setSpawnPos(glm::vec3 pos)
{
	m_pos = pos;
}

void ParticleSystem::startDying()
{
	m_data.continuous = false;
	m_counter = 0;
	timer = 0;
}
