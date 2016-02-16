#version 410

//layout (location = 0) in vec2 UV; 
out vec4 fragment_color;


uniform int spotlightID;

uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowMap;

struct SpotLight
{
	vec3 Color;
	float DiffuseIntensity;
	vec3 Position;
	float AmbientIntensity;
	vec3 Direction;
	float Cutoff;
	vec4 attenuation;
};

layout (std140) uniform Light
{ 
	SpotLight lights[500];
};

vec2 gScreenSize = vec2(1280, 720);

void main()
{
	vec2 UV = gl_FragCoord.xy / gScreenSize;
	fragment_color = vec4(lights[spotlightID].Color, 1) * 0.1 + texture(Diffuse, UV);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}