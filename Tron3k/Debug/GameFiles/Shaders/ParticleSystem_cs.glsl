#version 430
#define WORKGROUP_SIZE 8
#define PIXEL_PER_THREAD 4

struct Particle
{
    vec4 pos;
	//float intensity; == dir.w
	vec4 dir; // w == 0.0f
	vec4 initialDir; // w == 0.0f
};

layout(std430, binding = 1) buffer ParticleBuffer
{
    Particle p[];
};

uniform float lifetime; // when reset full intensity
uniform float deltaTime;
uniform float force;
uniform float drag;
uniform float gravity;
uniform float emission;

uniform bool continuous;
uniform bool omni;

uniform vec3 initialPos; // w == 0.0f

shared uint indexCounter;
shared uint arrayLenght;
shared uint passCount;

const uint threadCount = (WORKGROUP_SIZE * WORKGROUP_SIZE);

layout (local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE) in;
void main()
{
	// shared setup
	if(gl_LocalInvocationIndex == 0)
	{
		indexCounter = 0;
		arrayLenght = p.length();
		passCount = (arrayLenght + threadCount - 1) / threadCount;
	}
	
	barrier();
	
	// for each move it, reduce intensity, (when render check intensity)
	// if continuous reset particle
	
	for(uint iter = gl_LocalInvocationIndex; iter < arrayLenght; iter+=threadCount)
	{
		p[iter].dir.w -= deltaTime;
		
		//p[iter].pos.xyz = p[iter].pos.xyz + p[iter].dir.xyz*force;
		p[iter].pos.y -= gravity*deltaTime;
		//if(continuous)
		//{
			if(p[iter].dir.w < -1.0f)
			{
				//p[iter].dir = p[iter].initialDir;
				p[iter].pos.xyz = vec3(0,0,0);
				p[iter].dir.w = lifetime;
			}
		//}
	}
	
	
}