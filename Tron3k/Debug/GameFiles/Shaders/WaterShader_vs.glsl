#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 vertex_uv;

layout (location = 0) out vec4 pos;
layout (location = 1) out vec2 uv;
layout (location = 2) out vec2 uv2;

uniform mat4 MVP;
uniform float anim;
uniform int type;

void main()
{
	//Create
	gl_Position = MVP * vec4(pos, 1);
	uv = vertex_uv;
	uv2 = vertex_uv;
	
	//Sign
	if (type == 0)
	{
		uv.x = uv.x + anim;
	}

	//Water
	if (type == 1)
	{
		uv.y = uv.y - anim / 10;
		uv2.x = uv2.x - anim / 2;
	}
}