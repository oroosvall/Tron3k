#version 410

layout(location = 0) in vec2 uv_frag;
layout(location = 1) in float intenisty;

layout(location = 2) in vec3 wPos;
layout(location = 3) in vec3 normal;

uniform sampler2D tex;

uniform int glow;
uniform vec3 blendcolor;

layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main () 
{
	vec4 color = texture(tex, uv_frag);
	if(glow == 1)
	{
		NormalOut = vec4(0.0f,0.0f,0.0f, 0.0f);
		WorldPosOut = vec4(0.0f,0.0f,0.0f, 0.0f);
		DiffuseOut = vec4(0.0f,0.0f,0.0f, 0.0f);
		GlowMap = color * vec4(blendcolor, clamp(intenisty*3, 0.0f, 1.0f));
	}
	else
	{
		NormalOut = vec4(normal, color.w * clamp(intenisty*3, 0.0f, 1.0f));
		WorldPosOut = vec4(wPos, color.w * clamp(intenisty*3, 0.0f, 1.0f));
		DiffuseOut = color * vec4(blendcolor, clamp(intenisty*3, 0.0f, 1.0f));
		GlowMap = vec4(0,0,0,0);
	}
}