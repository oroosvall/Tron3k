#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) out vec2 UV;

//struct Decal 
//{
//	float inten;
//	vec3 pos;
//	vec3 color;
//	vec3 normal;
//	float dummy;
//	float dumm2;
//};
//
//layout(std140) uniform decal_in 
//{
//	Decal decals[50];
//};
//uniform int NrDecals;

uniform mat4 ViewProjMatrix;
uniform vec3 pos;
uniform vec3 normal;

vec3 right;
vec3 down;

void main() 
{	
	float size = 0.1f;
	
	if(normal.y > 0.99 || normal.y < -0.99)
	{
		right = vec3(1,0,0) * size;
		down = vec3(0,0,1) * size;
	}
	else
	{
		right = normalize(cross(normal, vec3(0,1,0))) * size;
		down = normalize(cross(normal, right)) * size;
	}
	
	UV = vec2(0,1);
	gl_Position = ViewProjMatrix * vec4(pos - down - right, 1);
	EmitVertex();
	UV = vec2(0,0);
	gl_Position = ViewProjMatrix * vec4(pos + down - right, 1);
	EmitVertex();
	UV = vec2(1,1);
	gl_Position = ViewProjMatrix * vec4(pos - down + right, 1);
	EmitVertex();
	UV = vec2(1,0);
	gl_Position = ViewProjMatrix * vec4(pos + down + right, 1);
	EmitVertex();
	
	EndPrimitive();
	
}

//	for(int n = 0; n < NrDecals; n++)
//	{	
//		Inten = decals[n].inten;
//		Color = decals[n].color;
//		
//		gl_Position = ViewProjMatrix * vec4(decals[n].pos + vec3(-1, 1, 0), 1);
//		EmitVertex();
//		gl_Position = ViewProjMatrix * vec4(decals[n].pos + vec3(-1, -1, 0), 1);
//		EmitVertex();
//		gl_Position = ViewProjMatrix * vec4(decals[n].pos + vec3(1, 1, 0), 1);
//		EmitVertex();
//		gl_Position = ViewProjMatrix * vec4(decals[n].pos + vec3(1, -1, 0), 1);
//		EmitVertex();
//		
//		EndPrimitive();
//		
//	}
