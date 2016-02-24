#version 410
layout(points) in; //points?
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) out vec2 UV;
layout (location = 1) out vec3 worldPos;

uniform vec2 UVset;

uniform mat4 world;
uniform mat4 vp;

void main() 
{
		worldPos = world[0].xyz;
		gl_Position = vp * world[0];
		UV = vec2(UVset.x, 0);
		EmitVertex();

		worldPos = world[1].xyz;
		gl_Position = vp * world[1];
		UV = vec2(UVset.x, 1);
		EmitVertex();

		worldPos = world[2].xyz;
		gl_Position = vp * world[2];
		UV = vec2(UVset.y, 0);
		EmitVertex();

		worldPos = world[3].xyz;
		gl_Position = vp * world[3];
		UV = vec2(UVset.y, 1);
		EmitVertex();
		
		EndPrimitive();
}


















