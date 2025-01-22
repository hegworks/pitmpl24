#version 310 es

precision highp float;

// in
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D uNoiseTexture;

void main()
{
    FragColor = texture(uNoiseTexture, ioTexCoord);
}
