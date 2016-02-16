#version 410
in vec2 uv;
in vec2 uv2;

uniform sampler2D tex;

out vec4 fragment_color;
void main () 
{
	fragment_color = texture(tex, uv2);

	float distance = gl_FragCoord.z / gl_FragCoord.w;
	float newdistance;
	float near = 7.0f;
	float far = 10.0f;

	//Make the distance a manageable value (colors range from 0 to 1, so we want this close to that range)
	newdistance = distance / 25;

	//If the color value is  66% or more bright
	if ((fragment_color.x + fragment_color.y + fragment_color.z) > 2.0f)
	{				
		fragment_color = fragment_color + newdistance;

		//Only a certain depth range gets fake specular. 0.4 -> 0.6
		if (newdistance < 0.5f && newdistance > 0.3f)
		{
			//Add fake specular
			fragment_color += newdistance * 1.3f;
		}
	}


	//Sample the same texture again with opposite animated UV coords,
	//this creates a "double layer" effect
	fragment_color = fragment_color * texture(tex, uv);

	//Restrict darkness and light. More "newdistance" = more darkness.			
	newdistance = clamp(newdistance, 0.0f, 0.35f);
			
	//Darken based on distance. Further back = darker
	fragment_color -= newdistance * 1.5f;


}