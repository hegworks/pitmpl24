#pragma once

#include "IInputKey.h"
#include "RoomChangeType.h"
#include <string>

namespace Uknitty
{

#pragma region Do NOT comment out lines here, use the next section
#define DEBUG_DRAW_PHYSICS
#pragma endregion Do NOT comment out lines here, use the next section

#pragma region to use a feature, comment out its line
#undef DEBUG_DRAW_PHYSICS
#pragma endregion to use a feature, comment out its line

#pragma region Screen
#ifdef WINDOWS_BUILD
constexpr int SCRWIDTH = 1600;
constexpr int SCRHEIGHT = 900;
#endif // WINDOWS_BUILD
#ifdef Raspberry_BUILD
constexpr int SCRWIDTH = 1280;
constexpr int SCRHEIGHT = 720;
#endif // Raspberry_BUILD
constexpr float ASPECT_RATIO = 16.0f / 9.0f;
constexpr float LOW_RES_SCALE = 0.25f;
#pragma endregion Screen

#pragma region Physics
//constexpr float PHYSICS_TIMESTEP = (1.0f / 30.0f); // 1.0f/x -> x times per second
//constexpr float PHYSICS_MAX_SUB_STEPS = 5; // bullet sub-steps per frame
constexpr float GRAVITY = -9.81f;

enum class PhysicsType
{
	SOLID,
	PLAYER,
	ROOM_CHANGE,
	ENEMY,
	WIN,
};

struct UserPointerData
{
	PhysicsType physicsType;
	RoomChangeType roomChangeType;
	std::string name;
	void* extraData;  // Optional additional data
};
#pragma endregion Physics

#pragma region GeneralCamera
const float MOUSE_SENSITIVITY = 0.05f;
const Key CAMERA_TYPE_SWITCH_KEY = Key::V;
#pragma endregion GeneralCamera

} // namespace Uknitty
