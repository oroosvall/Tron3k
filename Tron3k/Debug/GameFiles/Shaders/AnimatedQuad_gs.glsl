#version 410
layout(points) in; //points?
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) out vec2 UV;

uniform vec2 UVset;

uniform mat4 world;
uniform mat4 vp;

void main() 
{
		gl_Position = vp * world[0];
		UV = vec2(0, 0);
		EmitVertex();
		
		gl_Position = vp * world[1];
		UV = vec2(0, 1);
		EmitVertex();

		gl_Position = vp * world[2];
		UV = vec2(1, 0);
		EmitVertex();
		
		gl_Position = vp * world[3];
		UV = vec2(1, 1);
		EmitVertex();
		
		EndPrimitive();
}


















