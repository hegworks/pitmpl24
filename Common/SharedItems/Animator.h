#pragma once

#include "Animation.h"
#include "AssimpNodeData.h"
#include "Bone.h"
#include "BoneInfo.h"
#include "glm/glm.hpp"
#include <cmath>
#include <map>
#include <string>
#include <vector>

/// <summary>
/// This will read the hierarchy of AssimpNodeData, Interpolate all bones in a recursive manner and then prepares final bone transformation matrices.
/// </summary>

namespace Uknitty
{

namespace SkeletalAnimation
{

class Animator
{
public:
	/// <summary>
	/// takes an animation to play
	/// </summary>
	void Initialize(Animation* animation);
	void UpdateAnimation(float deltaTime);
	void PlayAnimation(Animation* animation, bool once = false);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4>* GetFinalBoneMatrices() { return &m_finalBoneMatrices; }

private:
	std::vector<glm::mat4> m_finalBoneMatrices;
	Animation* m_currentAnimation = nullptr;
	float m_currentTime;
	float m_deltaTime;
	bool m_isPlayOnceMode = false;
	bool m_hasPlayedOnce = false;
	const int MAX_BONES = 100;
};

inline void Animator::Initialize(Animation* animation)
{
	m_currentTime = 0.0;
	m_currentAnimation = animation;

	m_finalBoneMatrices.reserve(MAX_BONES);

	for(int i = 0; i < MAX_BONES; i++)
		m_finalBoneMatrices.push_back(glm::mat4(1.0f));
}

inline void Animator::UpdateAnimation(float deltaTime)
{
	m_deltaTime = deltaTime;

	if(!m_currentAnimation)
	{
		return;
	}

	if(m_isPlayOnceMode && m_hasPlayedOnce)
	{
		return;
	}

	m_currentTime += m_currentAnimation->GetTicksPerSecond() * deltaTime;
	if(m_currentTime >= m_currentAnimation->GetDuration())
	{
		if(m_isPlayOnceMode)
		{
			m_currentTime = m_currentAnimation->GetDuration() - 0.01f;
			m_hasPlayedOnce = true;
		}
		else
		{
			m_currentTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
		}
	}

	CalculateBoneTransform(&m_currentAnimation->GetRootNode(), glm::mat4(1.0f));
}

inline void Animator::PlayAnimation(Animation* animation, bool once)
{
	if(animation == m_currentAnimation)
	{
		return;
	}

	m_currentAnimation = animation;
	m_currentTime = 0.0f;
	m_isPlayOnceMode = once;
	m_hasPlayedOnce = false;
}

inline void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	// check if this node is engaged in the current animation
	Bone* bone = m_currentAnimation->FindBone(nodeName);
	if(bone)
	{
		bone->Update(m_currentTime);
		nodeTransform = bone->GetLocalTransform(); // local transform of a bone in bone space
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform; // apply parent's transformation to this node

	std::map<std::string, BoneInfo> boneInfoMap = m_currentAnimation->GetBoneInfoMap();
	if(boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_finalBoneMatrices[index] = globalTransformation * offset; // transform from mesh space to bone space
	}

	for(int i = 0; i < node->childrenCount; i++)
		CalculateBoneTransform(&node->children[i], globalTransformation);
}

} // namespace SkeletalAnimation

} // namespace Uknitty
