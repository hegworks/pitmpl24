#version 310 es

#define MAX_LIGHTS_COUNT 5
#define POINT_LIGHT 0
#define DIR_LIGHT 1
#define SPOT_LIGHT 2

precision highp float;
precision highp int;

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoord;

out vec2 ioTexCoord;
out vec3 ioLightingColor;
out vec3 ioFragPos;
out vec3 ioNormal;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uModelNormal;

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
    float attConst;
    float attLin;
    float attQuad;
    float cutOff;
};
uniform Light lights[MAX_LIGHTS_COUNT];

vec3 CalcDirLight(Light light, vec3 normal)
{
    vec3 lightDir = normalize(-light.dir);
    float angleDifference = max(dot(normal, lightDir), 0.0);
    return angleDifference * light.diffuseColor;
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos)
{
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));
    vec3 lightDir = normalize(light.pos - fragPos);
    float angleDifference = max(dot(normal, lightDir), 0.0);
    return angleDifference * light.diffuseColor * attenuation;
}

void main()
{
    vec3 fragPos = vec3(uModel * vec4(iPos, 1.0));
    vec3 normal = normalize(mat3(uModelNormal) * iNormal);

    vec3 sum = global.ambientStrength * global.ambientColor;

    for(int i = 0; i < global.lightsCount; i++)
    {
        Light light = lights[i];
        if(light.type == POINT_LIGHT)
        {
            sum += CalcPointLight(light, normal, fragPos);
        }
        else if(light.type == DIR_LIGHT)
        {
            sum += CalcDirLight(light, normal);
        }
    }

    ioLightingColor = sum;
    ioTexCoord = iTexCoord;
    ioFragPos = fragPos;
    ioNormal = normal;
    gl_Position = uMVP * vec4(iPos, 1.0);
}
