#pragma once

#define COLL_MASK_ALL				0xFFFFFFFFu	// All groups (all 32 bits 1)
#define COLL_MASK_NONE				0x0			// No groups (all 32 bits 0)
#define COLL_MASK_ALL_EXCEPT(x)		(~(x))		// All except the specified group(s)
#define COLL_MASK_NONE_EXCEPT(x)	(x)			// Only the specified group(s)

#define COLL_GROUP_PLAYER		(1 << 0)    // 1
#define COLL_GROUP_ENEMY		(1 << 1)    // 10
#define COLL_GROUP_BULLET		(1 << 2)    // 100
#define COLL_GROUP_OBSTACLE		(1 << 3)    // 1000
#define COLL_GROUP_PICKUP		(1 << 4)    // 10000

#define COLL_MASK_PLAYER		COLL_MASK_ALL_EXCEPT(COLL_GROUP_PLAYER)
#define COLL_MASK_ENEMY			COLL_MASK_ALL_EXCEPT(COLL_GROUP_ENEMY)
#define COLL_MASK_BULLET		COLL_MASK_ALL_EXCEPT(COLL_GROUP_BULLET)
#define COLL_MASK_OBSTACLE		COLL_MASK_ALL_EXCEPT(COLL_GROUP_OBSTACLE)
#define COLL_MASK_PICKUP		COLL_MASK_NONE_EXCEPT(COLL_GROUP_PLAYER)
