#version 410

layout(location = 0) in float dist;

out vec4 fragment_color;

void main () 
{
	float maxlen = 100;
	float transp = dist;
	
	if(transp > maxlen * 0.9)
		transp = 0.1;
	else
		transp =  1 - (transp / maxlen);

    fragment_color = vec4((1 - transp) / 1.5 , 0 , transp / 2, 0.25 + transp / 4);
}