#version 410
layout(location = 0);
layout(location = 1);
layout(location = 2);
layout(location = 3);
layout(location = 4);
layout(location = 5);

uniform mat4 ViewProjMatrix;
uniform mat4 BoneMatrices[100];
uniform int BoneMatricesIndices[];

void main()
{
	gl_Position = ViewProjMatrix * (vec4(PositionOut, 1.0) * BoneMatrices[]);
}