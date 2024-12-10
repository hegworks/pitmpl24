#pragma once

#include "glm.hpp"
#include "RoomChangePositionType.h"
#include "RoomChangeType.h"

struct RoomChange
{
	glm::vec2 newPlayerPos;
	RoomChangeType type;
	int nextRoomId;
	RoomChangePositionType positionType;
};
