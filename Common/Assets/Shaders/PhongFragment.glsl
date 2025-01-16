#version 310 es

precision highp float;

// in
in vec2 ioTexCoord;
in vec3 ioNormal;
in vec3 ioFragPos;
in mat4 ioModel;

// out
out vec4 FragColor;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Global
{
    float ambientStrength;
    vec3 ambientColor;
    vec3 viewPos;
};
uniform Global global;

struct Light
{
    vec3 pos;
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;
};
uniform Light light;

struct DirLight
{
    vec3 direction;
    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;
};  
uniform DirLight dirLight;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);
    vec3 ambientColor = global.ambientStrength * global.ambientColor;
    vec3 viewDir = normalize(global.viewPos - ioFragPos);

    // diffuse
    vec3 lightDir = normalize(light.pos - ioFragPos);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    // specular
    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
    vec3 specularColor = light.specularStrength * specular * light.specularColor;

    vec3 result = (ambientColor + diffuse + specularColor) * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}
