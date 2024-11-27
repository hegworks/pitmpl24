#version 310 es

precision highp float;

// in
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

// out
out vec2 ioTexCoord;

// uniform
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
   gl_Position = uProjection * uView * uModel * vec4(iPos, 1.0);
   ioTexCoord = iTexCoord;
}
