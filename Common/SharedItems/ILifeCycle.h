#pragma once

namespace Uknitty
{

class ILifeCycle
{
public:
	~ILifeCycle() = default;

	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;

private:

};

}
