#version 410
layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

//layout (location = 0) out vec2 UV; 

uniform mat4 ViewProjMatrix;
uniform int spotlightID;

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


vec3 right;
vec3 down;

void main() 
{	

	float spotlength = 20.0f;
	float halfwidth = 20.0f;
	
	vec3 spot_pos = lights[spotlightID].Position;
	vec3 spot_normal = lights[spotlightID].Direction * spotlength;
	
	right = normalize(cross(spot_normal, vec3(0,1,0))) * halfwidth;
	down = normalize(cross(spot_normal, right)) * halfwidth;
	
	//0
	gl_Position = ViewProjMatrix * vec4(spot_pos - down - right + spot_normal, 1);
	EmitVertex();
	//1
	gl_Position = ViewProjMatrix * vec4(spot_pos, 1);
	EmitVertex();
	//2
	gl_Position = ViewProjMatrix * vec4(spot_pos - down + right + spot_normal, 1);
	EmitVertex();
	//3
	gl_Position = ViewProjMatrix * vec4(spot_pos + down + right + spot_normal, 1);
	EmitVertex();
	//4
	gl_Position = ViewProjMatrix * vec4(spot_pos - down - right + spot_normal, 1);
	EmitVertex();
	//5
	gl_Position = ViewProjMatrix * vec4(spot_pos + down - right + spot_normal, 1);
	EmitVertex();
	//6
	gl_Position = ViewProjMatrix * vec4(spot_pos, 1);
	EmitVertex();
	//7
	gl_Position = ViewProjMatrix * vec4(spot_pos + down + right + spot_normal, 1);
	EmitVertex();
	
	EndPrimitive();
	
}
