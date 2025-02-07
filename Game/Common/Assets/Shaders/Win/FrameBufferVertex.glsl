#version 310 es

precision highp float;

layout (location = 0) in vec2 iPos;
layout (location = 1) in vec2 iTexCoords;

out vec2 ioTexCoords;

void main()
{
    gl_Position = vec4(iPos.x, iPos.y, 0.0, 1.0);
    ioTexCoords = iTexCoords;
}
