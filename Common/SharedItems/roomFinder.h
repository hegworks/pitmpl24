#pragma once

#include "RoomChange.h"

class RoomChangeStorage;

class RoomFinder
{
public:
	RoomFinder();
	RoomFinder(RoomChangeStorage* roomChangeStorage);
	RoomChange* FindNextRoom(RoomChangeType roomChangeType) const;
	void SetCurrentLevelId(const int levelId) { m_currentLevelId = levelId; }
	int GetCurrentLevelId() const { return m_currentLevelId; }

private:
	RoomChangeStorage* m_roomChangeStorage;
	int m_currentLevelId = 0;
};
