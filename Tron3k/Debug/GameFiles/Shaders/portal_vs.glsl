#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out float dist;

uniform mat4 world;
uniform mat4 VP;

void main () 
{
    gl_Position =  VP * (vec4(vertex_position, 1) * world);
	
	dist = length(gl_Position);
}