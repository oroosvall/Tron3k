#version 420                                                               
layout (location = 0) in vec2 UV;                                                                  

uniform sampler2D normalSample;

uniform vec3 dynamicGlowColor;
uniform float timepass;  
  
layout (location = 4) out vec4 GlowMap;

vec4 normalMap;
vec4 glowspec;
float glow;

//layout(early_fragment_tests) in;

void main()									
{			
	glow = texture(normalSample, vec2(UV.x + timepass, 1-UV.y)).w;
	GlowMap = vec4((1.0f - glow) * dynamicGlowColor, 0.9f);
}