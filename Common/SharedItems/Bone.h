#pragma once

#include "assimp_glm_helpers.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <assimp/scene.h>
#include <cassert>
#include <string>
#include <vector>

#ifdef Raspberry_BUILD
#define GLM_ENABLE_EXPERIMENTAL
#endif // Raspberry_BUILD
#include "glm/gtx/quaternion.hpp"
#include <stdexcept>

namespace Uknitty
{

namespace SkeletalAnimation
{

/// <summary>
/// at what point of an animation (timestamp) we need to interpolate to its value (position)
/// </summary>
struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

/// <summary>
/// at what point of an animation (timestamp) we need to interpolate to its value (rotation)
/// </summary>
struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

/// <summary>
/// at what point of an animation (timestamp) we need to interpolate to its value (scale)
/// </summary>
struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

/// <summary>
/// A single bone which reads all keyframes data from aiNodeAnim.
/// It will also interpolate between its keys: Translation, Scale and Rotation based on the current animation time.
/// </summary>
class Bone
{
public:
	/// <summary>
	/// <para>
	///		extracts keys and their timestamps from aiNodeAnim channels and stores them.
	/// </para>
	/// <para>
	///		aiNodeAnim channel: a single bone's animation keys and their timestamps.
	/// </para>
	/// </summary>
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	~Bone() = default;

	/// <summary>
	/// interpolates between positions, rotations and scales keys based on the curren time of the animation
	/// and prepares the m_localTransform matrix (matrix to transform local bone to nodeTransform) by combining all keys tranformations.
	/// </summary>
	void Update(float animationTime);

	/// <summary>
	/// returns the matrix that transforms the bone from its local space to the node space.
	/// </summary>
	glm::mat4 GetLocalTransform() const { return m_localTransform; }
	std::string GetBoneName() const { return m_name; }
	int GetBoneID() const { return m_id; }

	/// <summary>
	/// gets the currnet index in the m_positions array based on the current animation time; to interpolate to.
	/// </summary>
	int GetPositionIndex(float animationTime);
	/// <summary>
	/// gets the currnet index in the m_rotations array based on the current animation time; to interpolate to.
	/// </summary>
	int GetRotationIndex(float animationTime);
	/// <summary>
	/// gets the currnet index in the m_scales array based on the current animation time; to interpolate to.
	/// </summary>
	int GetScaleIndex(float animationTime);

private:
	/// <summary>
	/// gets normalized value for Lerp and Slerp
	/// </summary>
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	/// <summary>
	/// figures out which position keys to interpolate between and performs the interpolation and returns the translation matrix
	/// </summary>
	glm::mat4 InterpolatePosition(float animationTime);
	/// <summary>
	/// figures out which rotation keys to interpolate between and performs the interpolation and returns the rotation matrix
	/// </summary>
	glm::mat4 InterpolateRotation(float animationTime);
	/// <summary>
	/// figures out which scale keys to interpolate between and performs the interpolation and returns the scale matrix
	/// </summary>
	glm::mat4 InterpolateScale(float animationTime);

	std::vector<KeyPosition> m_positions;
	std::vector<KeyRotation> m_rotations;
	std::vector<KeyScale> m_scales;

	int m_numPositions;
	int m_numRotations;
	int m_numScales;

	glm::mat4 m_localTransform; // matrix to transform local bone to nodeTransform
	std::string m_name;
	int m_id;
};


inline Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel) :
	m_name(name),
	m_id(id),
	m_localTransform(1.0f)
{
	m_numPositions = channel->mNumPositionKeys;
	for(int positionIndex = 0; positionIndex < m_numPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = static_cast<float>(channel->mPositionKeys[positionIndex].mTime);
		KeyPosition data;
		data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		m_positions.push_back(data);
	}

	m_numRotations = channel->mNumRotationKeys;
	for(int rotationIndex = 0; rotationIndex < m_numRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = static_cast<float>(channel->mRotationKeys[rotationIndex].mTime);
		KeyRotation data;
		data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		m_rotations.push_back(data);
	}

	m_numScales = channel->mNumScalingKeys;
	for(int keyIndex = 0; keyIndex < m_numScales; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = static_cast<float>(channel->mScalingKeys[keyIndex].mTime);
		KeyScale data;
		data.scale = AssimpGLMHelpers::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		m_scales.push_back(data);
	}
}

inline void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScale(animationTime);
	m_localTransform = translation * rotation * scale;
}

inline int Bone::GetPositionIndex(float animationTime)
{
	for(int index = 0; index < m_numPositions - 1; ++index)
	{
		if(animationTime < m_positions[index + 1].timeStamp)
			return index;
	}
	throw std::runtime_error("Error: Position index at animationTIme not found");
}

inline int Bone::GetRotationIndex(float animationTime)
{
	for(int index = 0; index < m_numRotations - 1; ++index)
	{
		if(animationTime < m_rotations[index + 1].timeStamp)
			return index;
	}
	throw std::runtime_error("Error: Rotation index at animationTIme not found");
}

inline int Bone::GetScaleIndex(float animationTime)
{
	for(int index = 0; index < m_numScales - 1; ++index)
	{
		if(animationTime < m_scales[index + 1].timeStamp)
			return index;
	}
	throw std::runtime_error("Error: Scale index at animationTIme not found");
}

inline float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

inline glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if(m_numPositions == 1)	return glm::translate(glm::mat4(1.0f), m_positions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_positions[p0Index].timeStamp, m_positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(m_positions[p0Index].position, m_positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

inline glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if(m_numRotations == 1)
	{
		glm::highp_quat rotation = glm::normalize(m_rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_rotations[p0Index].timeStamp, m_rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(m_rotations[p0Index].orientation, m_rotations[p1Index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

inline glm::mat4 Bone::InterpolateScale(float animationTime)
{
	if(m_numScales == 1) return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(m_scales[p0Index].scale, m_scales[p1Index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}

} // namespace SkeletalAnimation

} // namespace Uknitty
