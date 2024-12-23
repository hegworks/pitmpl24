#pragma once

namespace Uknitty
{

#pragma region Do NOT comment out lines here, use the next section
#define VISUAL_LEAK_DETECTOR
#define DEBUG_DRAW_PHYSICS
#pragma endregion Do NOT comment out lines here, use the next section

#pragma region to use a feature, comment out its line
#undef VISUAL_LEAK_DETECTOR
#undef DEBUG_DRAW_PHYSICS
#pragma endregion to use a feature, comment out its line

constexpr float PHYSICS_TIMESTEP = 1.0f / 30.0f; //		x/y		y times every x second
constexpr float GRAVITY = -9.81f;

} // namespace Uknitty
