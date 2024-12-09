#version 310 es

precision highp float;

// in
layout (location = 0) in vec3 iPos;

// uniform
uniform mat4 uMVP;

void main()
{
   gl_Position = uMVP * vec4(iPos.x, iPos.y, iPos.z, 1.0);
}
