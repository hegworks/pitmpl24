#include "PerlinNoiseManager.h"

#include "AssetManager.h"
#include "Engine.h"
#include "RNG.h"
#include "ShaderProgram.h"
#include "ShaderType.h"

PerlinNoiseManager::PerlinNoiseManager()
{
	m_shaderProgram = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::PERLIN, VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	// Create vertex data for a quad
	Vertex vertices[] = {
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
		{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
	};

	// Create and bind VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create and fill VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
}

void PerlinNoiseManager::Update(float deltaTime)
{
	m_time += deltaTime;
}

void PerlinNoiseManager::GenerateNewPerlinNoiseTexture(const int width, const int height, const int octaves)
{
	Uknitty::RNG::numOctaves = octaves;

	// Create buffer for texture data
	std::vector<unsigned char> textureData(width * height * 4);

	// Generate noise values
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float noiseValue = Uknitty::RNG::noise2D(x + m_time * SPEED, y + m_time * SPEED);
			noiseValue = (noiseValue + 1.0f) * 0.5f;

			unsigned char pixelValue = static_cast<unsigned char>(noiseValue * 255);

			int index = (y * width + x) * 4;
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}
