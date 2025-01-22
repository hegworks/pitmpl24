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

	void GenerateNewPerlinNoiseTexture(const int gridSize, const int octaves);
	void GenerateNewPerlinNoiseMesh(const int gridSize, const int octaves);
	Uknitty::ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
	unsigned int GetTextureId() const { return m_textureId; }
	unsigned int GetTextureVAO() const { return m_textureVAO; }
	unsigned int GetMeshVAO() const { return m_meshVAO; }
	unsigned int GetIndexCount() const { return m_indexCount; }

private:
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;

	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Shaders/PerlinFragment.glsl";
	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Shaders/PerlinVertex.glsl";

	unsigned int m_textureId = 0;
	unsigned int m_meshVAO = 0;
	unsigned int m_textureVAO = 0;
	float m_time = 0;
	int m_indexCount = 0;
};
