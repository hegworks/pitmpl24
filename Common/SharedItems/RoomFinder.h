﻿#pragma once

#include "GameSettings.h"
#include "RoomChange.h"

class RoomChangeStorage;

class RoomFinder
{
public:
	RoomFinder();
	~RoomFinder();

	RoomChange* FindNextRoom(RoomChangeType roomChangeType) const;
	void SetCurrentLevelId(const int levelId) { m_currentLevelId = levelId; }
	int GetCurrentLevelId() const { return m_currentLevelId; }

private:
	RoomChangeStorage* m_roomChangeStorage;
	int m_currentLevelId = STARTING_MAP_ID;
};
