#version 310 es

// Pi

precision highp float;

// in
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

// out
out vec2 ioTexCoord;
out vec3 ioNormal;
out vec3 ioFragPos;
out mat4 ioModel;

// uniform
uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uModelNormal;

void main()
{
	gl_Position = uMVP * vec4(iPos, 1.0);
	ioFragPos = vec3(uModel * vec4(iPos, 1.0));
	ioModel = uModel;
	ioTexCoord = iTexCoord;
	ioNormal = normalize(mat3(uModelNormal) * iNormal);
}
