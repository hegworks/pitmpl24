#version 310 es

precision highp float;

// out
out vec4 oFragColor;

// uniform
uniform vec3 uColor;

void main()
{
   oFragColor = vec4(uColor, 1.0f);
}
