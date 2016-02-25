#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (location = 0) out vec2 UV;

uniform vec3 pos;
uniform vec3 dir; //sets length and dir (inverse)
uniform vec3 crossAngle; //sets upangle and halfwidth

uniform mat4 vp;

void main() 
{
		
		gl_Position = vp * vec4( pos - crossAngle, 1);
		UV = vec2(0, 1);
		EmitVertex();

		gl_Position = vp * vec4( pos + crossAngle, 1);
		UV.y = 0;
		EmitVertex();

		gl_Position = vp * vec4( pos - crossAngle + dir, 1);
		UV = vec2(1, 1);
		EmitVertex();

		gl_Position = vp * vec4( pos + crossAngle + dir, 1);
		UV.y = 0;
		EmitVertex();
		
		EndPrimitive();
}


















