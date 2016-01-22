#version 410
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 WorldMatrix;		

out vec2 UV;

void main () {
	UV = vertex_uv;
	gl_Position = vec4 (vertex_position, 0.0, 1.0) * WorldMatrix;
}