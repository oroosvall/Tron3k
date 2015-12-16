#version 410

layout (location = 0) in vec2 UV;

uniform float deltaTime;
uniform float falloff;
  
uniform sampler2D glowAdd;
  
layout (location = 1) out vec4 GlowMap;

void main()									
{	
	GlowMap += vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	vec4 addColor = texture(glowAdd, UV);
	
	GlowMap += vec4(addColor.rgb, addColor.w*0.01);
	
	
}