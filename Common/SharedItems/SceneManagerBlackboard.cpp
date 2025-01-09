#include "SceneManagerBlackboard.h"

#include "GameSharedDependencies.h"

SceneManagerBlackboard::SceneManagerBlackboard()
{
	GameSharedDependencies::Set<SceneManagerBlackboard>(this);
}
