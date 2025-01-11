#pragma once

#include "RoomChangePositionType.h"
#include "RoomChangeType.h"
#include <glm/glm.hpp>

struct RoomChange
{
	glm::vec2 newPlayerPos;
	RoomChangeType type;
	int nextRoomId;
	RoomChangePositionType positionType;
};
