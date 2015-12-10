#version 430

uniform mat4 inverseBindpose[100];

struct bones
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec4 boneIndices;
	vec4 skinWeights;
};

layout (std430, binding = 2) buffer boneArray
{
	bones data[];
};	

layout (local_size_x = 20, local_size_y = 1, local_size_z = 1)in;  //This row isn't done

void main()
{
	uint boneId =  gl_GlobalInvocationID.x;

	bones[boneId].pos = (vec4(bones[boneId].pos, 1.0f) * inverseBindpose[bones[boneId].boneIndices.x]) * bones[boneId].skinWeights.x;
	bones[boneId].normal = (vec4(bones[boneId].normal, 1.0f) * inverseBindpose[bones[boneId].boneIndices.x]) * bones[boneId].skinWeights.x;
	bones[boneId].tangent = (vec4(bones[boneId].tangent, 1.0f) * inverseBindpose[bones[boneId].boneIndices.x]) * bones[boneId].skinWeights.x;
	
	bones[boneId].pos = (vec4(bones[boneId].pos, 1.0f) * inverseBindpose[bones[boneId].boneIndices.y]) * bones[boneId].skinWeights.y;
	bones[boneId].normal = (vec4(bones[boneId].normal, 1.0f) * inverseBindpose[bones[boneId].boneIndices.y]) * bones[boneId].skinWeights.y;
	bones[boneId].tangent = (vec4(bones[boneId].tangent, 1.0f) * inverseBindpose[bones[boneId].boneIndices.y]) * bones[boneId].skinWeights.y;
	
	bones[boneId].pos = (vec4(bones[boneId].pos, 1.0f) * inverseBindpose[bones[boneId].boneIndices.z]) * bones[boneId].skinWeights.z;
	bones[boneId].normal = (vec4(bones[boneId].normal, 1.0f) * inverseBindpose[bones[boneId].boneIndices.z]) * bones[boneId].skinWeights.z;
	bones[boneId].tangent = (vec4(bones[boneId].tangent, 1.0f) * inverseBindpose[bones[boneId].boneIndices.z]) * bones[boneId].skinWeights.z;
	
	bones[boneId].pos = (vec4(bones[boneId].pos, 1.0f) * inverseBindpose[bones[boneId].boneIndices.w]) * bones[boneId].skinWeights.w;
	bones[boneId].normal = (vec4(bones[boneId].normal, 1.0f) * inverseBindpose[bones[boneId].boneIndices.w]) * bones[boneId].skinWeights.w;
	bones[boneId].tangent = (vec4(bones[boneId].tangent, 1.0f) * inverseBindpose[bones[boneId].boneIndices.w]) * bones[boneId].skinWeights.w;
}