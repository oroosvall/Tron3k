#version 430
layout(location = 0) in vec3 VertexPos;
layout(location = 1) in vec2 VertexUV;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;

layout(location = 0) out vec3 PositionOut;
layout(location = 1) out vec2 UVOut;

void main()
{
	PositionOut = (vec4(VertexPos, 1.0) * WorldMatrix).xyz;
	gl_position = ProjMatrix * ViewMatrix * vec4(PositionOut, 1.0)
	UVOut = VertexUV;
}