#version 310 es

precision highp float;

// in
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

// out
out vec2 ioTexCoord;
out vec3 ioPos;

// uniform
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(iPos, 1.0);
	ioTexCoord = iTexCoord;
	ioPos = iPos;
}
