#version 420                                                               
layout (location = 0) in vec2 UV;                                                                  

uniform sampler2D normalSample;

uniform vec3 dynamicGlowColor;
uniform float timepass;
uniform float inten;  
  
layout (location = 4) out vec4 GlowMap;

vec4 normalMap;
vec4 glowspec;
float glow;
vec4 color;

//layout(early_fragment_tests) in;

void main()									
{	
	color = texture(normalSample, vec2(UV.x + timepass * 0.1f, 1-UV.y + timepass * 0.2f));
	color +=  vec4((1 - texture(normalSample, vec2(UV.x + timepass * 0.3f, 1-UV.y)).w) * dynamicGlowColor, 0.5f);
	GlowMap =  color * vec4(dynamicGlowColor, 1) * inten * 0.7f;
}