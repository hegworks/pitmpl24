#pragma once

#include "IInputKey.h"

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

#pragma region Screen
constexpr int SCRWIDTH = 960;
constexpr int SCRHEIGHT = 540;
constexpr float ASPECT_RATIO = 16.0f / 9.0f;
#pragma endregion Screen

#pragma region Physics
constexpr float PHYSICS_TIMESTEP = 1.0f / 30.0f; //		x/y		y times every x second
constexpr float GRAVITY = -9.81f;
#pragma endregion Physics

#pragma region GeneralCamera
const float MOUSE_SENSITIVITY = 0.05f;
const Key CAMERA_TYPE_SWITCH_KEY = Key::V;
#pragma endregion GeneralCamera


} // namespace Uknitty
