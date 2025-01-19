#version 310 es

layout (location = 0) in vec3 iPos;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(iPos, 1.0);
}
