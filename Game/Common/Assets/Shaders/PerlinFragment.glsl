#version 310 es

#define SCROLL_SPEED 0.1

precision highp float;

// in
in vec2 ioTexCoord;
in vec3 ioPos;

// out
out vec4 FragColor;

// uniform
uniform sampler2D uNoiseTexture;
uniform float uTime;
uniform bool uIsMeshTrueIsTextureFalse;

void main()
{
    if(uIsMeshTrueIsTextureFalse)
    {
        FragColor = vec4(ioPos.y/3.5,ioPos.y/3.5,ioPos.y/1.5, 1.0);
    }
    else
    {
        // vec2 scrolledUV = ioTexCoord + vec2(cos(uTime) * SCROLL_SPEED, sin(uTime) * SCROLL_SPEED);
        vec2 scrolledUV = ioTexCoord + vec2(uTime * SCROLL_SPEED);
        FragColor = texture(uNoiseTexture, scrolledUV);
    }
}
