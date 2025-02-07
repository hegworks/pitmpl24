#version 310 es

precision highp float;

//in
in vec3 ioColor;

// out
out vec4 oFragColor;

void main()
{
   oFragColor = vec4(ioColor, 1.0f);
}
