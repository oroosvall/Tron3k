#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 WorldMatrix;
uniform vec3 pivot;		

out vec2 UV;

void main () {
	UV = vertex_uv; 
	gl_Position = vec4(vertex_position, 1.0)  * WorldMatrix - vec4(pivot, 0.0);
	
}