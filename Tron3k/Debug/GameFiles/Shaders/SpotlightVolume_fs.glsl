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

vec3 Position0;
vec4 Normal0;
vec4 GlowSpec0;
vec4 Diffuse0;

float gSpecularPower = 200;
float gMatSpecularIntensity = 0.4;

void main()
{

	vec2 UV = gl_FragCoord.xy / gScreenSize;
	//fragment_color = vec4(lights[spotlightID].Color, 1) * 0.1 + texture(Diffuse, UV);
	
	Position0 = texture(Position, UV).xyz; // sampling
                                                                                     
	vec3 LightToPixel = normalize(Position0 - lights[spotlightID].Position);                             
	float SpotFactor = dot(LightToPixel, lights[spotlightID].Direction);                                      
    	
	if (SpotFactor > lights[spotlightID].Cutoff) 
	{                                                            
		vec3 LightDirection = Position0 - lights[spotlightID].Position;
		float Distance = length(LightDirection);
		LightDirection = normalize(LightDirection);    
		
		Normal0 = texture(Normal, UV); // sampling
		
		//==========================
		// CalcLightInternal
		//==========================

		float DiffuseFactor = dot(Normal0.xyz, -LightDirection);                                                                                                            
		vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                                                             
                                                                                
		if (DiffuseFactor > 0) 
		{                                                            
			fragment_color = vec4(lights[spotlightID].Color, 1.0f) * (lights[spotlightID].DiffuseIntensity * 3) * DiffuseFactor;    
			
			//==========================
			//		Specular
			//==========================
			vec3 eyepos = vec3(0,0,0); // Uniform this into the shader!
			
			vec3 VertexToEye = normalize(eyepos - Position0);                             
			vec3 LightReflect = normalize(reflect(LightDirection, Normal0.xyz));                     
			float SpecularFactor = dot(VertexToEye, LightReflect);                              
			SpecularFactor = pow(SpecularFactor, gSpecularPower); 
			vec4 specularAddetive;
			
			if (SpecularFactor > 0)
			{
				float Distance = length(Position0 - lights[spotlightID].Position);
				float Attenuation = 1.0f / pow(max(0.0f, 1.0f - (Distance/80)), 5);
				specularAddetive = (vec4(lights[spotlightID].Color, 1.0f) * ( 1 - Normal0.w) * SpecularFactor) / Attenuation;
			}
			
			//==========================
			//	Spotlight Attenuation
			//==========================	
			float Attenuation = pow(max(0.0f, 1.0f - (Distance/80)), 10);
			fragment_color *= Attenuation;	
			
			
			//==========================
			//	Cone Falloff
			//==========================	
			fragment_color = fragment_color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - lights[spotlightID].Cutoff));
			
			
			//==========================
			//	Frag color finalize
			//==========================
			Diffuse0 = texture(Diffuse, UV);
			fragment_color = fragment_color * Diffuse0 + specularAddetive;
						
		}	
		else
		{
			fragment_color = vec4(0,0,0,0);
		}
	}	
	else
	{	
		fragment_color = vec4(0,0,0,0);
	}
}