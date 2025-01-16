#version 310 es

#define POINT_LIGHT 0
#define DIR_LIGHT 1
#define SPOT_LIGHT 2

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
    int type;

    vec3 diffuseColor;
    vec3 specularColor;
    float specularStrength;
    float shininess;

    vec3 pos;
    vec3 dir;

    // PointLight
    float attConst;
    float attLin;
    float attQuad;

    // SpotLight
    float cutOff;
    float outerCutOff;
};
uniform Light light;

// struct PointLight
// {
//     vec3 diffuseColor;
//     vec3 specularColor;
//     float specularStrength;
//     float shininess;
// 
//     vec3 pos;
//     float attConst;
//     float attLin;
//     float attQuad;
// };
// uniform PointLight pointLight;
// 
// struct DirLight
// {
//     vec3 direction;
// 
//     vec3 diffuseColor;
//     vec3 specularColor;
//     float specularStrength;
//     float shininess;
// };  
// uniform DirLight dirLight;
// 
// struct SpotLight
// {
//     vec3 pos;
//     vec3 direction;
//     float cutOff;
// 
//     vec3 diffuseColor;
//     vec3 specularColor;
//     float specularStrength;
//     float shininess;
// };  
// uniform SpotLight spotLight;

vec3 CalcPointLight(vec3 viewDir);
vec3 CalcDirLight(vec3 viewDir);
vec3 CalcSpotLight(vec3 viewDir);

void main()
{
    vec3 sum = vec3(0.0);
    vec3 viewDir = normalize(global.viewPos - ioFragPos);
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);

    vec3 ambientColor = global.ambientStrength * global.ambientColor;
    sum += ambientColor;

    if(light.type == POINT_LIGHT)
    {
        sum += CalcPointLight(viewDir);
    }
    else if(light.type == DIR_LIGHT)
    {
        sum += CalcDirLight(viewDir);
    }
    else if(light.type == SPOT_LIGHT)
    {
        sum += CalcSpotLight(viewDir);
    }

    vec3 result = sum * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 viewDir)
{
    vec3 sum = vec3(0.0);

    vec3 lightDir = normalize(-light.dir);
    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    sum += diffuse;

    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
    vec3 specularColor = light.specularStrength * specular * light.specularColor;
    sum += specularColor;

    return sum;
}

vec3 CalcPointLight(vec3 viewDir)
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

vec3 CalcSpotLight(vec3 viewDir)
{
    vec3 sum = vec3(0.0);

    vec3 lightDir = normalize(light.pos - ioFragPos);

    float theta = dot(lightDir, normalize(-light.dir));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
    
    float distance = length(light.pos - ioFragPos);
    float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));

    float angleDifference = max(dot(ioNormal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    sum += diffuse * attenuation * intensity;

    vec3 reflectDir = reflect(-lightDir, ioNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
    vec3 specularColor = light.specularStrength * specular * light.specularColor;
    sum += specularColor * attenuation * intensity;

    return sum;
}

