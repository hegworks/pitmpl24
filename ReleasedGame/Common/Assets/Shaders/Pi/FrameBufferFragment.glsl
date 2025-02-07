#version 310 es

precision highp float;

in vec2 ioTexCoords;

out vec4 FragColor;

uniform sampler2D uScreenTexture;

void main()
{
	vec4 textureColor = texture(uScreenTexture, ioTexCoords);

	vec3 result = vec3(textureColor);

	FragColor = vec4(result, 1.0);
}
