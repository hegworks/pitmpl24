#version 310 es

#define SCROLL_SPEED 0.5

precision highp float;

// in
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D uNoiseTexture;
uniform float uTime;

void main()
{
    vec2 scrolledUV = ioTexCoord + vec2(uTime * SCROLL_SPEED);
    FragColor = texture(uNoiseTexture, scrolledUV);
}
