#version 310 es

#define SCROLL_SPEED 0.1

precision highp float;

// in
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoord;

// out
out vec2 ioTexCoord;
out vec3 ioPos;

// uniform
uniform mat4 uMVP;
uniform float uTime;
uniform bool uIsMeshTrueIsTextureFalse;

void main()
{
	if(uIsMeshTrueIsTextureFalse)
	{
		vec3 wavePos = iPos;
		float waveHeight = 0.5 * sin(iPos.x + uTime * 2.0) * sin(iPos.z + uTime * 1.5);
		wavePos.y += waveHeight;
		gl_Position = uMVP * vec4(wavePos, 1.0);
	}
	else
	{
		gl_Position = uMVP * vec4(iPos, 1.0);
	}
	ioTexCoord = iTexCoord;
	ioPos = iPos;
}
