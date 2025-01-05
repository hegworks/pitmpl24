#include "RoomFinder.h"

#include "RoomChangeStorage.h"

RoomFinder::RoomFinder(RoomChangeStorage* roomChangeStorage)
{
	m_roomChangeStorage = roomChangeStorage;
}

RoomFinder::~RoomFinder()
{
	delete m_roomChangeStorage;
}

RoomFinder::RoomFinder()
{
	m_roomChangeStorage = new RoomChangeStorage();
}

RoomChange* RoomFinder::FindNextRoom(RoomChangeType roomChangeType) const
{
	return &m_roomChangeStorage->roomChanges[m_currentLevelId][static_cast<int>(roomChangeType)];
}
