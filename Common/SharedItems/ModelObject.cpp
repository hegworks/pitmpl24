#include "ModelObject.h"

#include "CRender.h"
#include "GameObject.h"
#include "ShaderProgram.h"

namespace Uknitty
{

void ModelObject::Initialize(Model* model, ShaderProgram* shaderProgram)
{
	GameObject::GetCRender()->Initialize(model, shaderProgram);
}

const ShaderProgram& ModelObject::GetShaderProgram()
{
	return *GameObject::GetCRender()->m_shaderProgram;
}

void ModelObject::OnAwake()
{
	GameObject::AddCRender();
}

} // namespace Uknitty
