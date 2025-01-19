#version 310 es
#define MAX_LIGHTS_COUNT 5
#define SPOT_LIGHT 2
precision highp float;
precision highp int;

in vec2 ioTexCoord;
in vec3 ioLightingColor;
in vec3 ioFragPos;
in vec3 ioNormal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

struct Light {
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

struct Global {
    float ambientStrength;
    vec3 ambientColor;
    vec3 viewPos;
    int lightsCount;
};
uniform Global global;

vec3 CalcSpotLight(Light light) {
    vec3 lightDir = normalize(light.pos - ioFragPos);
    float theta = dot(lightDir, normalize(-light.dir));
    if(theta > light.cutOff) {
        float distance = length(light.pos - ioFragPos);
        float attenuation = 1.0 / (light.attConst + light.attLin * distance + light.attQuad * (distance * distance));
        float angleDifference = max(dot(ioNormal, lightDir), 0.0);
        return angleDifference * light.diffuseColor * attenuation;
    }
    return vec3(0.0);
}

void main() {
    vec3 sum = ioLightingColor;
    
    for(int i = 0; i < global.lightsCount; i++) {
        Light light = lights[i];
        if(light.type == SPOT_LIGHT) {
            sum += CalcSpotLight(light);
        }
    }
    
    vec4 textureColor = texture(texture_diffuse1, ioTexCoord);
    FragColor = vec4(sum * vec3(textureColor), 1.0);
}