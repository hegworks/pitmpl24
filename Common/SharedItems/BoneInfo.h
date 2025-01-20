#pragma once

#include "glm/glm.hpp"

namespace Uknitty
{

namespace SkeletalAnimation
{

struct BoneInfo
{
	int id; // bone index in the finalBonesMatrices in vertex shader
	glm::mat4 offset; // the matrix to transform vertex from mesh/model/local space to bone space
};

} // namespace SkeletalAnimation

} // namespace Uknitty
