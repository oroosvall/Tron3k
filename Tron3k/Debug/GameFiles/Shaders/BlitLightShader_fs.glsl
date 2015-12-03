#version 410
layout (location = 0) in vec2 UV;
layout (location = 1) in vec2 blur[25];
uniform int Use;
	
uniform sampler2D Position;
uniform sampler2D Diffuse;
uniform sampler2D Normal;
uniform sampler2D GlowSpec;
uniform sampler2D GlowMap;	
uniform sampler2D Depth;

vec4 Position0;
vec4 Diffuse0;
vec4 Normal0;
vec4 Depth0;
vec4 glowValue;

struct SpotLight
{
	vec3 Color;
	float DiffuseIntensity;
	vec3 Position;
	float AmbientIntensity;
	vec3 Direction;
	float Cutoff;
};

layout (std140) uniform Light
{ 
	SpotLight lights[500];
};

uniform int NumSpotLights;
uniform vec3 eyepos;

float gSpecularPower = 20;
float gMatSpecularIntensity = 0.4;

out vec4 fragment_color;
					
vec4 CalcLightInternal(SpotLight l, vec3 LightDirection, vec3 Normal)                   
{                                                                                           
	              
	float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                           
	vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                            
	vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  
                                                                                           
	if (DiffuseFactor > 0) 
	{                                                                
		DiffuseColor = vec4(l.Color, 1.0f) * l.DiffuseIntensity * DiffuseFactor;    
                                                                                           
		vec3 VertexToEye = normalize(eyepos - Position0.xyz);                             
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
		float SpecularFactor = dot(VertexToEye, LightReflect);                              
		SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
		if (SpecularFactor > 0)                                                           
			SpecularColor = vec4(l.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor; 				
	}                                                                                                                                                                         
	return (DiffuseColor + SpecularColor);                                   
}               

vec4 CalcPointLight(SpotLight l, vec3 Normal)
{
	vec3 LightDirection = Position0.xyz - l.Position;
	float Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);    

	vec4 Color = vec4(CalcLightInternal(l, LightDirection, Normal)); 
	float Attenuation =  0.1 * Distance;
	return Color / Attenuation;     
	//float att = pointlightArray[i].attenuation.x + (pointlightArray[i].attenuation.y * dist) + (pointlightArray[i].attenuation.z * dist * dist);			
}                                                                                           
                                                                                           
vec4 CalcSpotLight(SpotLight l, vec3 Normal)                                                
{                                                                                           
	vec3 LightToPixel = normalize(Position0.xyz - l.Position);                             
	float SpotFactor = dot(LightToPixel, l.Direction);                                      
    	
	if (SpotFactor > l.Cutoff) 
	{                                                            
		vec4 Color = CalcPointLight(l, Normal);                             
		return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   
	}                                                                                       
	else                                                                                
		return vec4(0,0,0,0);                                                                   
	
}  		

void main()
{
	if(Use == 0)
	{
		float Depth = texture(Depth, vec2(UV.x, UV.y)).x;
		fragment_color = vec4(1.0 - (1.0 - Depth) * 25.0); 
	}
	else if(Use == 1)
		fragment_color = texture(Position, vec2(UV.x, UV.y)) * 0.25;
	else if(Use == 2)	 
		fragment_color = texture(Diffuse, vec2(UV.x, UV.y));
	else if(Use == 3)	 
		fragment_color = texture(Normal, vec2(UV.x, UV.y));
	else if(Use == 4)	 
		fragment_color = texture(GlowMap, vec2(UV.x, UV.y));
	else if(Use == 5)
	{
		fragment_color = vec4(0,0,0,0);
		Diffuse0 = texture(Diffuse, vec2(UV.x, UV.y));
		Position0 = texture(Position, vec2(UV.x, UV.y));
		Normal0 = texture(Normal, vec2(UV.x, UV.y));
		Depth0 = texture(Depth, vec2(UV.x, UV.y));
		glowValue = texture(GlowMap, vec2(UV.x, UV.y));
			
		for(int n = 0; n < NumSpotLights; n++)
		{
			fragment_color += CalcSpotLight(lights[n], Normal0.xyz);
			fragment_color += Diffuse0 * vec4(lights[n].Color, 1) * lights[n].AmbientIntensity;
		}
		
		fragment_color = fragment_color * Diffuse0;
		
	
		int count = 0;
	
		for(int x = 0; x < 12; x++)
		{
			fragment_color += texture(GlowMap, blur[count]) * 0.1 * (1 - ((( 6 - x ) * ( 6 - x )) / 36));
			count++;
		}	
		for(int y = 0; y < 12; y++)
		{
			fragment_color += texture(GlowMap, blur[count]) * 0.1 * (1 - ((( 6 - y ) * ( 6 - y )) / 36));
			count++;
		}
	

		////add glow
		//fragment_color += texture(GlowMap, blur[0])*0.0044299121055113265;
		//fragment_color += texture(GlowMap, blur[1])*0.00895781211794;
		//fragment_color += texture(GlowMap, blur[2])*0.0215963866053;
		//fragment_color += texture(GlowMap, blur[3])*0.0443683338718;
		//fragment_color += texture(GlowMap, blur[4])*0.0776744219933;
		//fragment_color += texture(GlowMap, blur[5])*0.115876621105;
		//fragment_color += texture(GlowMap, blur[6])*0.147308056121;
		//fragment_color += texture(GlowMap, UV)		*0.159576912161;
		//fragment_color += texture(GlowMap, blur[7])*0.147308056121;
		//fragment_color += texture(GlowMap, blur[8])*0.115876621105;
		//fragment_color += texture(GlowMap, blur[9])*0.0776744219933;
		//fragment_color += texture(GlowMap, blur[10])*0.0443683338718;
		//fragment_color += texture(GlowMap, blur[11])*0.0215963866053;
		//fragment_color += texture(GlowMap, blur[12])*0.00895781211794;
		//fragment_color += texture(GlowMap, blur[13])*0.0044299121055113265;
		//
		//fragment_color += texture(GlowMap, blur[14])*0.0044299121055113265;
		//fragment_color += texture(GlowMap, blur[15])*0.00895781211794;
		//fragment_color += texture(GlowMap, blur[16])*0.0215963866053;
		//fragment_color += texture(GlowMap, blur[17])*0.0443683338718;
		//fragment_color += texture(GlowMap, blur[18])*0.0776744219933;
		//fragment_color += texture(GlowMap, blur[19])*0.115876621105;
		//fragment_color += texture(GlowMap, blur[20])*0.147308056121;
		//fragment_color += texture(GlowMap, UV)		*0.159576912161;
		//fragment_color += texture(GlowMap, blur[21])*0.147308056121;
		//fragment_color += texture(GlowMap, blur[22])*0.115876621105;
		//fragment_color += texture(GlowMap, blur[23])*0.0776744219933;
		//fragment_color += texture(GlowMap, blur[24])*0.0443683338718;
		//fragment_color += texture(GlowMap, blur[25])*0.0215963866053;
		//fragment_color += texture(GlowMap, blur[26])*0.00895781211794;
		//fragment_color += texture(GlowMap, blur[27])*0.0044299121055113265;	
	}	
}