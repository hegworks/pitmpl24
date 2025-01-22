#pragma once

#include <string>

namespace Uknitty
{
class ShaderProgram;
}

class PerlinNoiseManager
{
public:
	PerlinNoiseManager();
	~PerlinNoiseManager() = default;

	void Update(float deltaTime);

	// Vertex data for a quad
	struct Vertex
	{
		float position[3];
		float texCoord[2];
	};

	void GenerateNewPerlinNoiseTexture(const int width, const int height, const int octaves);
	Uknitty::ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
	unsigned int GetTextureId() const { return m_textureId; }
	unsigned int GetVAO() const { return m_vao; }

private:
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;

	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Shaders/PerlinFragment.glsl";
	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Shaders/PerlinVertex.glsl";

	unsigned int m_textureId = 0;
	unsigned int m_vao = 0;
	unsigned int m_vbo = 0;
	float m_time = 0;
};
