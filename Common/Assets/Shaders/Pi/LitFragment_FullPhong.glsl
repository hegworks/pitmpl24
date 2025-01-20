#version 310 es

// Pi

#define MAX_LIGHTS_COUNT 4
#define POINT_LIGHT 0
#define DIR_LIGHT 1
#define SPOT_LIGHT 2

precision highp float;

// in
in vec2 ioTexCoord;
in vec3 ioNormal;
in vec3 ioFragPos;
in mat3 ioTBN;

// out
out vec4 FragColor;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform bool uHasNormalMap;

struct Global
{
    float ambientStrength;
    vec3 ambientColor;
    vec3 viewPos;
    int lightsCount;
};
uniform Global global;

struct Light
{
    int type;

    vec3 diffuseColor;

    vec3 pos;
    vec3 dir;

    // PointLight
    float attConst;
    float attLin;
    float attQuad;

    // SpotLight
    float cutOff;
};
uniform Light lights[MAX_LIGHTS_COUNT];

vec3 CalcPointLight(Light light, vec3 viewDir, vec3 normal);
vec3 CalcDirLight(Light light, vec3 viewDir, vec3 normal);
vec3 CalcSpotLight(Light light, vec3 viewDir, vec3 normal);

void main()
{
    vec3 sum = vec3(0.0);
    vec3 viewDir = normalize(global.viewPos - ioFragPos);
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);

    vec3 normal;
    if (uHasNormalMap)
    {
        // Use normal map
        normal = texture(texture_normal1, ioTexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(ioTBN * normal);
    }
    else
    {
        // Use vertex normal
        normal = ioNormal;
    }

    vec3 ambientColor = global.ambientStrength * global.ambientColor;
    sum += ambientColor;

    for(int i = 0; i < global.lightsCount; i++)
    {
        Light light = lights[i];
        if(light.type == POINT_LIGHT)
        {
            sum += CalcPointLight(light, viewDir, normal);
        }
        else if(light.type == DIR_LIGHT)
        {
            sum += CalcDirLight(light, viewDir, normal);
        }
        else if(light.type == SPOT_LIGHT)
        {
            sum += CalcSpotLight(light, viewDir, normal);
        }
    }

    vec3 result = sum * vec3(textureColor);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 viewDir, vec3 normal)
{
    vec3 sum = vec3(0.0);

    vec3 lightDir = normalize(-light.dir);
    float angleDifference = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    sum += diffuse;

    return sum;
}

vec3 CalcPointLight(Light light, vec3 viewDir, vec3 normal)
{
    vec3 sum = vec3(0.0);

    float distance = length(light.pos - ioFragPos);
    float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));

    vec3 lightDir = normalize(light.pos - ioFragPos);
    float angleDifference = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = angleDifference * light.diffuseColor;
    sum += diffuse * attenuation;

    return sum;
}

vec3 CalcSpotLight(Light light, vec3 viewDir, vec3 normal)
{
    vec3 sum = vec3(0.0);

    vec3 lightDir = normalize(light.pos - ioFragPos);
    float theta = dot(lightDir, normalize(-light.dir));

    if(theta > light.cutOff)
    {
        float distance = length(light.pos - ioFragPos);
        float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));

        float angleDifference = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = angleDifference * light.diffuseColor;
        sum += diffuse * attenuation;
    }

    return sum;
}

