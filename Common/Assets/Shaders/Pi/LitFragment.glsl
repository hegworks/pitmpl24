// Fragment Shader
#version 310 es
precision highp float;

in vec2 ioTexCoord;
in vec3 ioLightingColor;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main() {
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);
    FragColor = vec4(ioLightingColor * vec3(textureColor), 1.0);
}