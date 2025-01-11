#include "RoomFinder.h"

#include "RoomChangeStorage.h"

RoomFinder::RoomFinder()
{
	m_roomChangeStorage = new RoomChangeStorage();
}

RoomFinder::~RoomFinder()
{
	delete m_roomChangeStorage;
}

RoomChange* RoomFinder::FindNextRoom(RoomChangeType roomChangeType) const
{
	return &m_roomChangeStorage->roomChanges[m_currentLevelId][static_cast<int>(roomChangeType)];
}
