#version 430
layout(location = 0) in vec3 vertex_position;

uniform mat4 worldMat;
uniform mat4 project;
uniform mat4 view;

void main () 
{
    gl_Position =  view * project * worldMat * vec4(vertex_position, 1.0f);
}