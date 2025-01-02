#include "ModelObject.h"

#include "CRender.h"
#include "GameObject.h"

namespace Uknitty
{

void ModelObject::Initialize(Model* model, ShaderProgram* shaderProgram)
{
	GameObject::GetCRender()->Initialize(model, shaderProgram);
}

void ModelObject::OnAwake()
{
	GameObject::AddCRender();
}

} // namespace Uknitty
