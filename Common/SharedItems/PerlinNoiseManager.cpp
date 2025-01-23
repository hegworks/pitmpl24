#include "PerlinNoiseManager.h"

#include "AssetManager.h"
#include "Engine.h"
#include "RNG.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include "UknittyMath.h"
#include <vector>

PerlinNoiseManager::PerlinNoiseManager()
{
	m_shaderProgram = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::PERLIN, VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

void PerlinNoiseManager::Update(float deltaTime)
{
	m_time += deltaTime;
	m_shaderProgram->Use();
	m_shaderProgram->SetFloat("uTime", m_time);
	m_shaderProgram->UnUse();
}

void PerlinNoiseManager::GenerateNewPerlinNoiseTexture(const int gridSize, const int octaves)
{
	Uknitty::RNG::numOctaves = octaves;
	Uknitty::RNG::m_amplitude = 1;

	// Create vertex data for a quad
	Vertex vertices[] = {
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
		{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
	};

	// Create and bind VAO
	glGenVertexArrays(1, &m_textureVAO);
	glBindVertexArray(m_textureVAO);

	// Create and fill VBO
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	// Create buffer for texture data
	std::vector<unsigned char> textureData(gridSize * gridSize * 4);

	// Generate noise values
	for(int y = 0; y < gridSize; y++)
	{
		for(int x = 0; x < gridSize; x++)
		{
			float noiseValue = Uknitty::RNG::noise2D(x + m_time, y + m_time);
			noiseValue = (noiseValue + 1.0f) * 0.5f;

			unsigned char pixelValue = static_cast<unsigned char>(noiseValue * 255);

			int index = (y * gridSize + x) * 4;
			textureData[index + 0] = pixelValue;  // R
			textureData[index + 1] = pixelValue;  // G
			textureData[index + 2] = pixelValue;  // B
			textureData[index + 3] = 255;         // A
		}
	}

	// Generate texture
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Upload texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gridSize, gridSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void PerlinNoiseManager::GenerateNewPerlinNoiseMesh(const int gridSize, const int octaves)
{
	Uknitty::RNG::numOctaves = octaves;
	Uknitty::RNG::m_amplitude = 5;

	// Generate indices
	std::vector<unsigned int> indices;
	for(int z = 0; z < gridSize - 1; z++)
	{
		for(int x = 0; x < gridSize - 1; x++)
		{
			int topLeft = z * gridSize + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * gridSize + x;
			int bottomRight = bottomLeft + 1;

			// First triangle
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// Second triangle
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	// Create and bind VAO
	glGenVertexArrays(1, &m_meshVAO);
	glBindVertexArray(m_meshVAO);

	// Create and fill EBO
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	std::vector<Vertex> vertices(gridSize * gridSize);
	for(int z = 0; z < gridSize; z++)
	{
		for(int x = 0; x < gridSize; x++)
		{
			float zFloat = static_cast<float>(z);
			float xFloat = static_cast<float>(x);
			float noiseValue = Uknitty::RNG::noise2D(zFloat, xFloat);
			float height = Uknitty::Math::range_to_range(-1.0, 1.0, 0, 2.5, noiseValue);
			Vertex vertex;
			vertex.position[0] = static_cast<float>(x) - gridSize / 2.0f;
			vertex.position[1] = height;
			vertex.position[2] = static_cast<float>(z) - gridSize / 2.0f;
			vertex.texCoord[0] = static_cast<float>(x) / gridSize;
			vertex.texCoord[1] = static_cast<float>(z) / gridSize;
			vertices[z * gridSize + x] = vertex;
		}
	}

	// Create and fill VBO
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glBindVertexArray(0);

	// Set index count
	m_indexCount = static_cast<int>((gridSize - 1) * (gridSize - 1) * 6);
}
