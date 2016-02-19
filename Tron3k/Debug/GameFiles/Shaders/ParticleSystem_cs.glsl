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
uniform int emission;

uniform bool continuous;
uniform bool omni;

uniform vec3 initialPos;
uniform vec3 sysDir;

shared int emit;
shared uint nextEmitIndex;
shared uint arrayLenght;
shared uint passCount;

const uint threadCount = (WORKGROUP_SIZE * WORKGROUP_SIZE);

layout (local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE) in;
void main()
{
	// shared setup
	if(gl_LocalInvocationIndex == 0)
	{
		emit = emission;
		//nextEmitIndex = p[iter].pos.w; // ??
		arrayLenght = p.length();
		passCount = (arrayLenght + threadCount - 1) / threadCount;
	}
	
	barrier();
	
	// for each move it, reduce intensity, (when render check intensity)
	// if continuous reset particle
	
	for(uint iter = gl_LocalInvocationIndex; iter < arrayLenght; iter+=threadCount)
	{
		p[iter].dir.w -= deltaTime;
		
		float percent = p[iter].dir.w / lifetime;
		
		//p[iter].pos.xyz = p[iter].pos.xyz + p[iter].dir.xyz*force;
		
		vec3 oldP = p[iter].pos.xyz;
		vec3 vel = normalize( p[iter].initialDir.xyz)*deltaTime;
		
		p[iter].pos.y += ((-9.81f + percent*10 ) * gravity) * deltaTime;
		
		p[iter].pos.x -= vel.x * force;
		p[iter].pos.y += vel.y * force;
		p[iter].pos.z -= vel.z * force;
		
		p[iter].dir.xyz = normalize(oldP - p[iter].pos.xyz);
		
		
		//if(continuous)
		//{
			if(p[iter].dir.w < 0.0f)
			{
				//0 means all emit at the same pos
				if(emit == 0)
				{
					p[iter].dir = p[iter].initialDir;
					p[iter].pos.xyz = initialPos;
					p[iter].dir.w = lifetime;
				}
				
				//-1 means only one emits at a time
				else if (emit == -1)
				{
					//atomic
					int old = atomicExchange(emit, 1);
					if(old == -1)
					{
						p[iter].dir = p[iter].initialDir;
						p[iter].pos.xyz = initialPos;
						p[iter].dir.w = lifetime;
					}
				}
			}
		//}
	}
	
	
}