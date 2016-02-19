#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 20) out;

uniform mat4 ViewProjMatrix;
uniform int pointlightID;

struct SpotLight
{
	vec3 Color;
	float DiffuseIntensity;
	vec3 Position;
	float AmbientIntensity;
	vec3 Direction;
	float Cutoff;
	vec4 attenuation;
};

layout (std140) uniform Light
{ 
	SpotLight lights[500];
};

void main() 
{	
	float size = lights[pointlightID].attenuation.w;
	vec4 point_pos = vec4(lights[pointlightID].Position, 1);
	
	vec4 pos0 = ViewProjMatrix * (point_pos + vec4(size, size, -size, 0));
	vec4 pos1 = ViewProjMatrix * (point_pos + vec4(size, -size, -size, 0));
	vec4 pos2 = ViewProjMatrix * (point_pos + vec4(-size, size, -size, 0));
	vec4 pos3 = ViewProjMatrix * (point_pos + vec4(-size, -size, -size, 0));
	vec4 pos4 = ViewProjMatrix * (point_pos + vec4(-size, size, size, 0));
	vec4 pos5 = ViewProjMatrix * (point_pos + vec4(-size, -size, size, 0));
	vec4 pos6 = ViewProjMatrix * (point_pos + vec4(size, size, size, 0));
	vec4 pos7 = ViewProjMatrix * (point_pos + vec4(size, -size, size, 0));
	
	//0
	gl_Position = pos0;
	EmitVertex();
	//1
	gl_Position = pos1;
	EmitVertex();
	//2
	gl_Position = pos2;
	EmitVertex();
	//3
	gl_Position = pos3;
	EmitVertex();
	//4
	gl_Position = pos4;
	EmitVertex();
	//5
	gl_Position = pos5;
	EmitVertex();
	//6
	gl_Position = pos6;
	EmitVertex();
	//7
	gl_Position = pos7;
	EmitVertex();
	//8
	gl_Position = pos0;
	EmitVertex();
	//9
	gl_Position = pos1;
	EmitVertex();
	
	//10
	gl_Position = pos1;
	EmitVertex();
	//11
	gl_Position = pos7;
	EmitVertex();
	//12
	gl_Position = pos3;
	EmitVertex();
	//13
	gl_Position = pos5;
	EmitVertex();
	//14
	gl_Position = pos5;
	EmitVertex();
	//15
	gl_Position = pos4;
	EmitVertex();
	//16
	gl_Position = pos4;
	EmitVertex();
	//17
	gl_Position = pos6;
	EmitVertex();
	//18
	gl_Position = pos2;
	EmitVertex();
	//19
	gl_Position = pos0;
	EmitVertex();
	
	
	EndPrimitive();
	
}
