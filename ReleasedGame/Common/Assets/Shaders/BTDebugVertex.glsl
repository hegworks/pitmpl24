#version 310 es

precision highp float;

// in
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iColor;

// uniform
uniform mat4 uMVP;

// out
out vec3 ioColor;

void main()
{
   gl_Position = uMVP * vec4(iPos.x, iPos.y, iPos.z, 1.0);
   ioColor = iColor;
}
