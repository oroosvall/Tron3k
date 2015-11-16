#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (location = 0) in vec3 Position[3];
layout (location = 1) in vec2 UVCord[3];

layout (location = 0) out vec3 PositionOut;
layout (location = 1) out vec2 UVOut;
layout (location = 2) out vec3 NormalOut;

void main() 
{
	vec3 line1 = vec3(Position[1] - Position[0]);
	vec3 line2 = vec3(Position[2] - Position[0]);
	NormalOut = normalize(cross(line1, line2));

	for(int n = 0; n < 3; n++)
	{
		PositionOut = Position[n];
		gl_Position = gl_in[n].gl_Position;
		UVOut = UVCord[n];
		EmitVertex();
	}
	EndPrimitive();
}