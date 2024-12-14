#pragma once

#include "glm/glm.hpp"

//#define FULLSCREEN // uncomment to full screen
//#define VISUAL_LEAK_DETECTOR // uncomment to use VLD
//#define DEBUG_DRAW_PHYSICS
//#define DEBUG_DRAW_ASTAR_COLLISIONS
//#define DEBUG_DRAW_ASTAR_PATH
//#define DONT_DRAW_SOLIDS

constexpr int SCRWIDTH = 960;
constexpr int SCRHEIGHT = 540;
constexpr float ASPECT_RATIO = 16.0f / 9.0f;
constexpr int TILE_SIZE = 32;
constexpr int MAP_SCALE_X = 32;
constexpr int MAP_SCALE_Z = 24;
constexpr glm::vec2 MAP_CENTER = glm::vec2(MAP_SCALE_X / 2, MAP_SCALE_Z / 2);
constexpr int STARTING_MAP_ID = 2;
