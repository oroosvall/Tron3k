#version 410
layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 dir;
layout(location = 2) in vec4 iDir;

layout(location = 0) out vec4 dirOut;
layout(location = 1) out vec4 iDirOut;

uniform mat4 worldMat;
uniform mat4 view;

void main () 
{
	gl_Position =  view * vec4(vertex_position.xyz, 1.0f) * worldMat;
    dirOut = dir;
    iDirOut = iDir;
}