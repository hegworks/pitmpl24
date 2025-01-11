#include "SceneManagerBlackboard.h"

#include "GameSharedDependencies.h"

SceneManagerBlackboard::SceneManagerBlackboard()
{
	GameSharedDependencies::Set<SceneManagerBlackboard>(this);
}

SceneManagerBlackboard::~SceneManagerBlackboard()
{
	GameSharedDependencies::Remove<SceneManagerBlackboard>();
}
