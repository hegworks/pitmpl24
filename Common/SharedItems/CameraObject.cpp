#include "CameraObject.h"

#include "CTransform.h"
#include "GameObject.h"

namespace Uknitty
{

void CameraObject::UpdateLocalTransformMatrix(glm::mat4 viewProjection)
{
	GameObject::GetLocalTransform()->OverrideMatrix(viewProjection);
}

} // namespace Uknitty
