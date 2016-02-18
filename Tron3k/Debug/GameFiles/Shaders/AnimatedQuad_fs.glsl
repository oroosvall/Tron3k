#version 410
layout (location = 0) in vec2 uv;

uniform sampler2D tex;
//uniform int type;
    
layout (location = 4) out vec4 GlowMap;

void main () 
{
	GlowMap = texture(tex, uv);
	GlowMap.w = 0.8f;
	////Water Shader
	//if (type == 1)
	//{
	//	fragment_color = texture(tex, uv2);
    //
	//	float distance = gl_FragCoord.z / gl_FragCoord.w;
	//	float newdistance;
	//	float near = 7.0f;
	//	float far = 10.0f;
    //
	//	//Make the distance a manageable value (colors range from 0 to 1, so we want this close to that range)
	//	newdistance = distance / 25;
    //
	//	//If the color value is  66% or more bright
	//	if ((fragment_color.x + fragment_color.y + fragment_color.z) > 2.0f)
	//	{				
	//		fragment_color = fragment_color + newdistance;
    //
	//		//Only a certain depth range gets fake specular. 0.4 -> 0.6
	//		if (newdistance < 0.5f && newdistance > 0.3f)
	//		{
	//			//Add fake specular
	//			fragment_color += newdistance * 1.3f;
	//		}
	//	}
    //
    //
	//	//Sample the same texture again with opposite animated UV coords,
	//	//this creates a "double layer" effect
	//	fragment_color = fragment_color * texture(tex, uv);
    //
	//	//Restrict darkness and light. More "newdistance" = more darkness.			
	//	newdistance = clamp(newdistance, 0.0f, 0.35f);
	//		
	//	//Darken based on distance. Further back = darker
	//	fragment_color -= newdistance * 1.5f;
	//}
    //
	////Sign Shader
	//else if (type == 1)
	//{
	//	fragment_color = texture(tex, uv);
	//}
    //
    //
	//WorldPosOut = pos;
	//DiffuseOut = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//NormalOut = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	//GlowMap = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}