#version 310 es

// Pi

precision highp float;

// in
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitangent; 

layout (location = 5) in ivec4 iBoneIds; 
layout (location = 6) in vec4 iWeights;


// out
out vec2 ioTexCoord;
out vec3 ioNormal;
out vec3 ioFragPos;
out mat3 ioTBN;

// uniform
uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uModelNormal;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 uFinalBonesMatrices[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0f);
	bool hasAnimation = false;
	for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
	{
		if(iBoneIds[i] == -1) 
		{
			continue;
		}
		if(iBoneIds[i] >=MAX_BONES) 
		{
			totalPosition = vec4(iPos, 1.0f);
			break;
		}
		vec4 localPosition = uFinalBonesMatrices[iBoneIds[i]] * vec4(iPos, 1.0f);
		totalPosition += localPosition * iWeights[i];
		vec3 localNormal = mat3(uFinalBonesMatrices[iBoneIds[i]]) * iNormal;
		hasAnimation = true;
	}

	vec3 T = normalize(vec3(uModel * vec4(iTangent, 0.0)));
	vec3 B = normalize(vec3(uModel * vec4(iBitangent, 0.0)));
	vec3 N = normalize(vec3(uModel * vec4(iNormal, 0.0)));
	ioTBN = mat3(T, B, N);

	ioFragPos = vec3(uModel * vec4(iPos, 1.0));
	ioTexCoord = iTexCoord;
	ioNormal = normalize(mat3(uModelNormal) * iNormal);


	if(hasAnimation)
		gl_Position = uMVP * totalPosition;
	else
		gl_Position = uMVP * vec4(iPos, 1.0);
}
