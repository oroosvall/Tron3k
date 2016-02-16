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
	vec3 spot_pos = vec3(82, 8, 68);
	vec3 spot_normal = normalize(vec3(1, -1, 0));
	
	spot_pos = lights[spotlightID].Position;
	spot_normal = lights[spotlightID].Direction * 5;
	
	right = normalize(cross(spot_normal, vec3(0,1,0))) * 5;
	down = normalize(cross(spot_normal, right)) * 5;
	
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
