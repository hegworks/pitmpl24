#include "roomFinder.h"

#include "roomChangeStorage.h"

RoomFinder::RoomFinder(RoomChangeStorage* roomChangeStorage)
{
	m_roomChangeStorage = roomChangeStorage;
}

RoomChange RoomFinder::FindNextRoom(RoomChangeType roomChangeType) const
{
	return m_roomChangeStorage->roomChanges[m_currentLevelId][static_cast<int>(roomChangeType)];
}
