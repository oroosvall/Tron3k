#version 410
layout (location = 0) in vec2 UV;

uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowMap;	
uniform sampler2D Depth;

vec4 Position0;
vec4 Diffuse0;
vec4 Normal0;
vec4 Depth0;
vec4 glowValue;

uniform float pixeluvX;
uniform float pixeluvY;

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

uniform vec3 eyepos;

float gSpecularPower = 200.0f;
float gMatSpecularIntensity = 0.4f;
	
vec3 ambientLightPosition = vec3(131.0f, 45.0f, 0.0f);
vec3 ambientLightDirection = vec3(-0.16f, -0.36f, 0.61f);
vec3 ambientLightColor = vec3(0.7f, 0.7f , 1.0f);
float ambientLightDiffuseIntensity = 0.1f;
float ambientLightAmbientIntensity = 0.1f;

out vec4 fragment_color;                                                                                       

void main()
{
	fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
}