#pragma once
#include "glm.hpp"
#include "roomChangePositionType.h"
#include "roomChangeType.h"

struct RoomChange
{
	glm::vec2 newPlayerPos;
	RoomChangeType type;
	int nextRoomId;
	RoomChangePositionType positionType;
};
