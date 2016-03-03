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
uniform float spread;

uniform bool continuous;
uniform bool omni;

uniform vec3 initialPos;
uniform vec3 sysDir;

shared int emit;
shared uint nextEmitIndex;
shared uint arrayLenght;
shared uint passCount;

shared int maxemit;

const uint threadCount = (WORKGROUP_SIZE * WORKGROUP_SIZE);

layout (local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE) in;
void main()
{
	// shared setup
	if(gl_LocalInvocationIndex == 0)
	{
		maxemit = emit = emission;
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
		vec3 vel;
		if(!omni)
			vel = normalize( sysDir + (p[iter].initialDir.xyz * spread ))*deltaTime;
		else
			vel = normalize( p[iter].initialDir.xyz)*deltaTime;
		
		p[iter].pos.y += ((-9.81f + percent*10 ) * gravity) * deltaTime;
		
		p[iter].pos.x += vel.x * force;
		p[iter].pos.y += vel.y * force;
		p[iter].pos.z += vel.z * force;
		
		p[iter].dir.xyz = normalize(oldP - p[iter].pos.xyz);
		p[iter].initialDir.w = percent;
		
		if(continuous)
		{
			if(p[iter].dir.w < 0.0f)
			{
				//emit holt number of particles to emit this frame
				//atomic
				int old = atomicAdd(emit, -1);
				if(old > 0)
				{
					p[iter].dir = p[iter].initialDir;
					float dist = (old / maxemit);
					p[iter].pos.xyz = initialPos + vel* force;
					p[iter].dir.w = lifetime;
					
				}
			}
		}
		else
		{
			if(p[iter].dir.w < 0.0f && p[iter].dir.w > -1.0f)
			{
				p[iter].dir.w = -0.5f;
			}
			else if(p[iter].dir.w < -1.0f)
			{
				//atomic
				int old = atomicAdd(emit, -1);
				if(old > 0)
				{
					p[iter].dir = p[iter].initialDir;
					float dist = (old / maxemit);
					p[iter].pos.xyz = initialPos + vel * force;
					p[iter].dir.w = lifetime;
				}
			}
		}
	}
}