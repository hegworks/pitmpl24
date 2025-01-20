#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

namespace Uknitty
{

namespace SkeletalAnimation
{

/// <summary>
/// This struct helps us to isolate our Animation from Assimp
/// </summary>
struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

} // namespace SkeletalAnimation

} // namespace Uknitty
