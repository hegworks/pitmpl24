#version 310 es

precision highp float;

// in
in vec2 ioTexCoord;

// out
out vec4 FragColor;

// uniform
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    //FragColor = vec4(ioTexCoord,0,0);
    float ambientStrength = 0.2;
    vec3 lightColor = vec3 (0.0,1.0,0.0);
    vec3 ambient = ambientStrength * lightColor;
    FragColor = texture(texture_diffuse1, ioTexCoord) * vec4(ambient, 1.0);
}
