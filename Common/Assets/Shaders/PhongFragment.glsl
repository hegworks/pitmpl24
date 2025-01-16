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
    float attConst;
    float attLin;
    float attQuad;
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

vec3 CalcDirLight(vec3 viewDir);
vec3 CalcLight(vec3 viewDir);

void main()
{
    vec3 sum = vec3(0.0);
    vec3 viewDir = normalize(global.viewPos - ioFragPos);
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);

    vec3 ambientColor = global.ambientStrength * global.ambientColor;
    sum += ambientColor;

    sum += CalcDirLight(viewDir);

    sum += CalcLight(viewDir);

    vec3 result = sum * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 viewDir)
{
    vec3 sum = vec3(0.0);

    vec3 lightDir = normalize(-dirLight.direction);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * dirLight.diffuseColor;
    sum += diffuse;

    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), dirLight.shininess);
    vec3 specularColor = dirLight.specularStrength * specular * dirLight.specularColor;
    sum += specularColor;

    return sum;
}

vec3 CalcLight(vec3 viewDir)
{
    vec3 sum = vec3(0.0);

    float distance = length(light.pos - ioFragPos);
    float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));    

    vec3 lightDir = normalize(light.pos - ioFragPos);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    sum += diffuse * attenuation;

    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
    vec3 specularColor = light.specularStrength * specular * light.specularColor;
    sum += specularColor * attenuation;

    return sum;
}

