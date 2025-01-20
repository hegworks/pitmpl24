#pragma once

#include "assimp_glm_helpers.h"
#include "AssimpNodeData.h"
#include "Bone.h"
#include "BoneInfo.h"
#include "Model.h"
#include <assimp/anim.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>
#include <map>
#include <string>
#include <vector>

/// <summary>
/// An asset which reads data from aiAnimation and create a hierarchical record of Bones
/// </summary>

namespace Uknitty
{

namespace SkeletalAnimation
{

class Animation
{
public:
	Animation(const std::string& animationPath, Model* model);
	~Animation() = default;

	Bone* FindBone(const std::string& name);

	float GetTicksPerSecond() const { return m_ticksPerSecond; }
	float GetDuration() const { return m_duration; }
	const AssimpNodeData& GetRootNode() { return m_rootNode; }
	const std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_boneInfoMap; }

private:
	/// <summary>
	/// replicates Assimp's aiNode hierarchy and creates AssimpNodeDataHierarchy
	/// </summary>
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
	/// <summary>
	/// sometimes when a fbx model is loaded separetly, it doesn't have all the bones that an animation has.
	/// those bones could be found in the animation and not in the model. This function reads those bones from the animation.
	/// </summary>
	void ReadMissingBones(const aiAnimation* animation, Model& model);

	float m_duration; // how long the animation is
	int m_ticksPerSecond; // speed of the animation
	std::vector<Bone> m_bones;
	AssimpNodeData m_rootNode;
	std::map<std::string, BoneInfo> m_boneInfoMap;
};

inline Animation::Animation(const std::string& animationPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode); // make sure the animation exists
	auto animation = scene->mAnimations[0];
	m_duration = animation->mDuration;
	m_ticksPerSecond = animation->mTicksPerSecond;
	ReadHeirarchyData(m_rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

inline Bone* Animation::FindBone(const std::string& name)
{
	auto it = std::find_if(m_bones.begin(), m_bones.end(),
						   [&](const Bone& Bone)
						   {
							   return Bone.GetBoneName() == name;
						   });
	if(it == m_bones.end()) return nullptr;
	else return &(*it);
}

inline void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;
	auto& boneInfoMap = model.GetBoneInfoMap();
	int& boneCount = model.GetBoneCounter();

	//reading channels (bones engaged in an animation and their keyframes)
	for(int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if(boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		m_bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
	}

	m_boneInfoMap = boneInfoMap;
}

inline void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for(int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

} // namespace SkeletalAnimation

} // namespace Uknitty
