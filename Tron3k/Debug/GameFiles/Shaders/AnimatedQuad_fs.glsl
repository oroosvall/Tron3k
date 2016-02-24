#version 410
layout (location = 0) in vec2 uv;

uniform sampler2D tex;
uniform float timepass;
   
layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main () 
{
	if(timepass < 0)
	{
		GlowMap = texture(tex, uv);
		//DiffuseOut = GlowMap;
		//DiffuseOut.w = 1f;
		GlowMap.w = 0.9f;
	}
	else
	{
		//Set UV coords to base UV
		float tile = 20.0f;	
		vec2 UV1 = vec2(uv);
		vec2 UV2 = vec2(uv);

		//Tile UVs 
		UV1 *= tile;
		UV2 *= tile;

		//Animate UV1
		UV1.x += (timepass / 20);
		UV1.y += (timepass / 10);

		//Animate UV2
		UV2 -= (timepass / 30);


		GlowMap = texture(tex, UV1);
		GlowMap.w = 1.0f;

		float distance = gl_FragCoord.z / gl_FragCoord.w;
		float newdistance = distance / 50;
		float near = 7.0f;
		float far = 10.0f;

		if ((GlowMap.x + GlowMap.y + GlowMap.z) > 2.0f)
		{
			GlowMap = GlowMap + newdistance * 0.2f;

			if (newdistance < 5.0f && newdistance > 0.3f)
			{
				GlowMap += newdistance * 0.3f;
				GlowMap.w = newdistance;
			}
		}

		//Sample opposite
		GlowMap *= texture(tex, UV2);

		//Restrict dark & light
		newdistance = clamp(newdistance, 0.0f, 0.35f);
		
		//Darker based on distance
		GlowMap -= newdistance * 1.5f;
	}
}