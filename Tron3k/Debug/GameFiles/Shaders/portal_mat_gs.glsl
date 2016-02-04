#version 410
layout(points) in; //points?
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 world;
uniform mat4 VP;

void main() 
{
	//First triangle
	
		//first node
		gl_Position = vec4(pos1, 1);
		gl_Position.y += 1.3;
		gl_Position = ViewProjMatrix * gl_Position;
		UV = uv1 + vec2(0,1);
		EmitVertex();
		
		//second node
		gl_Position = vec4(pos1, 1);
		gl_Position.y -= 1.5;
		gl_Position = ViewProjMatrix * gl_Position;
		UV = uv1;
		EmitVertex();

		//third node
		gl_Position = vec4(pos2, 1);
		gl_Position.y += 1.3;
		gl_Position = ViewProjMatrix * gl_Position;
		UV = uv2 + vec2(0,1);
		EmitVertex();
		
		//third node
		gl_Position = vec4(pos2, 1);
		gl_Position.y -= 1.5;
		gl_Position = ViewProjMatrix * gl_Position;
		UV = uv2;
		EmitVertex();
		
		EndPrimitive();
}

















