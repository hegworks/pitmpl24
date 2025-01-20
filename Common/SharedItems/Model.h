#pragma once

#include "assimp_glm_helpers.h"
#include "BoneInfo.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>
#include <iostream>
#include <map>
#include <stb_image.h>
#include <stdexcept>
#include <string>
#include <vector>

class Shader;
struct Texture;

namespace Uknitty
{

class Model
{
public:
	Model(std::string const& path, glm::vec2 textureCoordScale = glm::vec2(1), bool shouldVerticallyFlipTexture = false)
	{
		stbi_set_flip_vertically_on_load(shouldVerticallyFlipTexture);
		m_textureCoordScale = textureCoordScale;
		loadModel(path);
	}
	~Model()
	{
		std::cout << "Destroying Model: " << m_directory << std::endl;
		for(unsigned int i = 0; i < textures_loaded.size(); i++)
		{
			glDeleteTextures(1, &textures_loaded[i].m_id);
		}
	}

	void Draw(ShaderProgram& shader);
	std::string GetDirectory() const { return m_directory; }
	std::string GetStrippedFileName() const;

#pragma region Bone
	std::map<std::string, SkeletalAnimation::BoneInfo>& GetBoneInfoMap() { return m_boneInfoMap; }
	int& GetBoneCounter() { return m_boneCounter; }
#pragma endregion


private:
	// model data
	std::vector<Mesh> meshes;
	std::string m_directory;
	std::vector<Texture> textures_loaded;

#pragma region Bone
	std::map<std::string, SkeletalAnimation::BoneInfo> m_boneInfoMap; // maps bone name to bone info
	int m_boneCounter = 0; // id of the next bone that we will add to m_boneInfoMap

	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
#pragma endregion

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	glm::vec2 m_textureCoordScale;
};

inline void Model::Draw(ShaderProgram& shader)
{
	for(unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

inline void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
	const aiScene* scene = importer.ReadFile(path, flags);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

inline std::string Model::GetStrippedFileName() const
{
	std::string result = m_directory.substr(m_directory.find_last_of('/') + 1);
	result = result.substr(result.find_last_of('\\') + 1);
	return result;
}

inline void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

inline Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates
		Vertex vertex;

		SetVertexBoneDataToDefault(vertex);

		vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if(mesh->mTextureCoords[0])
			vertex.m_texCoords = glm::vec2(mesh->mTextureCoords[0][i].x * m_textureCoordScale.x, mesh->mTextureCoords[0][i].y * m_textureCoordScale.y);
		else
			vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
		if(mesh->mTangents)
			vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		else
			vertex.m_tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		if(mesh->mBitangents)
			vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		else
			vertex.m_bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

	ExtractBoneWeightForVertices(vertices, mesh, scene);

	return Mesh(vertices, indices, textures);
}

inline unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	std::string fileName = directory + '/' + std::string(path);

	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if(!textureData)
	{
		std::cout << "ERROR::STBI::LOAD at file " << fileName << std::endl;
		throw std::runtime_error("ERROR::STBI::LOAD at file " + fileName);
	}

	GLenum format;
	if(nrChannels == 1)
		format = GL_RED;
	else if(nrChannels == 3)
		format = GL_RGB;
	else if(nrChannels == 4)
		format = GL_RGBA;
	else
	{
		std::cout << "ERROR::TextureFromFile::InvalidNrChannels at file " << fileName << std::endl;
		throw std::runtime_error("ERROR::TextureFromFile::InvalidNrChannels at file " + fileName);
	}

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);

	return texture;
}

inline std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool wasTextureLoadedBefore = false;
		for(unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if(std::strcmp(textures_loaded[j].m_path.data, str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				wasTextureLoadedBefore = true;
				break;
			}
		}
		if(!wasTextureLoadedBefore)
		{
			// if texture path contains backslash, change it to be the last part after backslash (fix for some fbx files)
			std::string texturePath = str.C_Str();
			size_t pos = texturePath.find_last_of('\\');
			if(pos != std::string::npos)
			{
				texturePath = texturePath.substr(pos + 1);
			}
			std::cout << "Loading: " << m_directory << "/" << texturePath.c_str() << std::endl;

			Texture texture;
			texture.m_id = TextureFromFile(texturePath.c_str(), m_directory);
			texture.m_type = typeName;
			texture.m_path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

inline void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_boneIDs[i] = -1;
		vertex.m_weights[i] = 0.0f;
	}
}

// Set the bone data to the vertex. If there is an empty slot, set the boneID and weight to that slot.
inline void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for(int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if(vertex.m_boneIDs[i] < 0)
		{
			vertex.m_weights[i] = weight;
			vertex.m_boneIDs[i] = boneID;
			break;
		}
	}
}

inline void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for(unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if(m_boneInfoMap.find(boneName) == m_boneInfoMap.end()) // if the bone is not found in the map, add new bone infos
		{
			SkeletalAnimation::BoneInfo newBoneInfo;
			newBoneInfo.id = m_boneCounter;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_boneInfoMap[boneName] = newBoneInfo;
			boneID = m_boneCounter;
			m_boneCounter++;
		}
		else // bone found, meaning it also influences vertices of mesh out of its scope
		{
			boneID = m_boneInfoMap[boneName].id;
		}
		assert(boneID != -1);

		aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;
		for(int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

} // namespace Uknitty
