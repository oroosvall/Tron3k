#version 410

layout (location = 0) in vec2 UV;

uniform sampler2D glowTexture;
  
layout (location = 5) out vec4 GlowMap;

void main()									
{	
	vec4 color = texture(glowTexture, UV);
	
	GlowMap = color;
		
}