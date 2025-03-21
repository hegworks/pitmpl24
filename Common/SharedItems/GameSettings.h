#pragma once

#include "glm/glm.hpp"
#include <string>

#pragma region Do NOT comment out lines here, use the next section
#define VISUAL_LEAK_DETECTOR
#define DEBUG_DRAW_ASTAR_COLLISIONS
#define DEBUG_DRAW_ASTAR_PATH
#pragma endregion Do NOT comment out lines here, use the next section

#pragma region to use a feature, comment out its line
#undef VISUAL_LEAK_DETECTOR
#undef DEBUG_DRAW_ASTAR_COLLISIONS
#undef DEBUG_DRAW_ASTAR_PATH
#pragma endregion to use a feature, comment out its line

#pragma region Release settings
constexpr int TILE_SIZE = 32;
constexpr int MAP_SCALE_X = 32;
constexpr int MAP_SCALE_Z = 24;
constexpr int STARTING_MAP_ID = 0;
#define MAP_CENTER glm::vec2(MAP_SCALE_X / 2.0f, MAP_SCALE_Z / 2.0f)
#define PLAYER_INITIAL_POS glm::vec3(0.0f, 2.0f, 0.0f)
#pragma endregion Release settings

#pragma region Debug settings
constexpr int DEBUG_MAX_ENEMIES_TO_SPAWN = 100;
#pragma endregion Debug settings
