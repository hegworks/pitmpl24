#version 310 es

precision highp float;

// in
in vec2 ioTexCoord;
in vec3 ioNormal;
in vec3 ioFragPos;
in mat4 ioModel;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform float uAmbientStrength;
uniform vec3 uAmbientColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;

// out
out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);

    vec3 ambientColor = uAmbientStrength * uAmbientColor;

    vec3 lightDir = normalize(uLightPos - ioFragPos);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * uLightColor;

    vec3 result = (ambientColor + diffuse) * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}
