#version 410
layout(points) in; //points?
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 world;
uniform mat4 vp;

void main() 
{
		gl_Position = vp * world[0];
		EmitVertex();
		
		gl_Position = vp * world[3];
		EmitVertex();

		gl_Position = vp * world[1];
		EmitVertex();
		
		gl_Position = vp * world[2];
		EmitVertex();
		
		EndPrimitive();
}


















