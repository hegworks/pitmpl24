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

uniform vec3 uViewPos;

// out
out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);

    vec3 ambientColor = uAmbientStrength * uAmbientColor;

    vec3 lightDir = normalize(uLightPos - ioFragPos);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * uLightColor;

    float specularStrength = 1.0;
    float shininess = 32.0;
    vec3 viewDir = normalize(uViewPos - ioFragPos);
    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularColor = specularStrength * specular * uLightColor;

    vec3 result = (ambientColor + diffuse + specular) * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}
