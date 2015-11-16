#verison 430
layout(location = 0) in vec3 VertexPos;
layout(location = 0) in vec2 VertexUV;

layout (location = 0) out vec2 UVOut;  

void main()
{
	gl_position = vec4(VertexPos ,1.0);
	UVOut = VertexUV;
}