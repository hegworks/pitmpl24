#version 310 es

precision highp float;

// in
in vec2 ioTexCoord;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float uAmbientStrength;
uniform vec3 uAmbientColor;

out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);
    vec4 ambientColor = vec4(uAmbientStrength * uAmbientColor, 1.0);
    FragColor = textureColor * ambientColor;
}
