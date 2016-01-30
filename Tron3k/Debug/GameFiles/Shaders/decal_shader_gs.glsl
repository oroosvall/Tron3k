#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) out vec2 UV;
layout (location = 1) out float Inten;
layout (location = 2) out vec3 Color;

struct Decal {
	float inten;
	vec3 pos;
	vec3 color;
	vec3 normal;
	float dummy;
	float dumm2;
};

layout(std140) uniform decal_in {
	Decal decals[50];
};

uniform int NrDecals;
uniform mat4 ViewProjMatrix;

void main() 
{
	for(int n = 0; n < NrDecals; n++)
	{
		Inten = decals[n].inten;
		Color = decals[n].color;
		
		vec3 right = cross(decals[n].normal, vec3(0,1,0));
		vec3 up = cross(right, decals[n].normal);
		
		gl_Position = ViewProjMatrix * vec4(decals[n].pos - right + up, 1);
		EmitVertex();
		gl_Position = ViewProjMatrix * vec4(decals[n].pos - right - up, 1);
		EmitVertex();
		gl_Position = ViewProjMatrix * vec4(decals[n].pos + right + up, 1);
		EmitVertex();
		gl_Position = ViewProjMatrix * vec4(decals[n].pos - right - up, 1);
		EmitVertex();
		
		EndPrimitive();
	}
}