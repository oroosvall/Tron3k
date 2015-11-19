#version 410
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ViewProjMatrix;

layout(location = 0) out vec3 PositionOut;
layout(location = 1) out vec2 UVOut;
layout(location = 2) out vec3 NormalOut;

void main()
{
	PositionOut = (vec4(VertexPos, 1.0) * WorldMatrix).xyz;
	gl_Position = ViewProjMatrix * vec4(PositionOut, 1.0);
	UVOut = VertexUV;
	NormalOut = vec4(VertexNormal, 1.0).xyz;
}