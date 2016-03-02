#version 410
layout (location = 0) in vec2 uv;
layout (location = 1) in vec3 worldPos;

uniform sampler2D tex;
uniform float timepass;
uniform int type;
   
layout (location = 1) out vec4 WorldPosOut;   
layout (location = 2) out vec4 DiffuseOut;     
layout (location = 3) out vec4 NormalOut;     
layout (location = 4) out vec4 GlowMap;

void main () 
{
	WorldPosOut = vec4(0);
	DiffuseOut = vec4(0);
	NormalOut = vec4(0);
	GlowMap = vec4(0);

	//Sign
	if(type == 0)
	{
		GlowMap = texture(tex, uv);
		//DiffuseOut = GlowMap;
		//DiffuseOut.w = 1f;
		GlowMap.w = 1.0f;
	}

	//Specular (Water normal)
	else if (type == 1)
	{
		DiffuseOut = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		WorldPosOut = vec4(worldPos, 1.0f);
		vec2 UV1 = vec2(uv);

		UV1 *= 20.0f;
		UV1.y += (timepass / 40);

		vec4 t = texture(tex, UV1);
		NormalOut = vec4(0, 1.0f, 0.3f, t.x);
		//NormalOut.w = 0.0f;
	}

	//Water
	else if (type == 2)
	{		
		
		//Set UV coords to base UV
		float tile = 20.0f;	
		vec2 UV1 = vec2(uv);
		vec2 UV2 = vec2(uv);

		//Tile UVs 
		UV1 *= tile;
		UV2 *= tile;

		//Animate UV1
		UV1.x += (timepass / 40);
		UV1.y += (timepass / 50);

		//Animate UV2
		UV2 -= (timepass / 50);


		GlowMap = texture(tex, UV1);
		GlowMap.w = 1.0f;

		float distance = gl_FragCoord.z / gl_FragCoord.w;
		float newdistance = distance / 50;
		float near = 7.0f;
		float far = 10.0f;

		if ((GlowMap.x + GlowMap.y + GlowMap.z) > 2.0f)
		{
			GlowMap.xyz = GlowMap.xyz + newdistance * 0.9f;

			if (newdistance > 0.2f && newdistance < 0.4f)
			{
				GlowMap += newdistance * 0.5f;
			}
		}

		//Sample opposite
		GlowMap *= texture(tex, UV2);

		//Restrict dark & light
		newdistance = clamp(newdistance, 0.1f, 0.4f);
		
		//Darker based on distance
		GlowMap -= newdistance * 1.8f;

		//GlowMap = vec4(0);
	}
}