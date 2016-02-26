#version 410

layout (location = 0) in vec2 UV;

uniform sampler2D glowTexture;
  
layout (location = 5) out vec4 GlowMap;

uniform float pixeluvX;
uniform float pixeluvY;

void main()									
{	
	vec4 color = vec4(0);
	//color = texture(glowTexture, vec2(UV.x - pixeluvX * -5, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * -4, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * -3, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * -2, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * -1, UV.y)) * 0.1f;
	//
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 0, UV.y)) * 0.1f;
	//
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 1, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 2, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 3, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 4, UV.y)) * 0.1f;
	//color += texture(glowTexture, vec2(UV.x - pixeluvX * 5, UV.y)) * 0.1f;
	
	color = texture(glowTexture, UV);
	GlowMap = color;
}