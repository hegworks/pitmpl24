#pragma once

namespace Uknitty
{

class IRenderable
{
public:
	~IRenderable() = default;

	virtual void Render() = 0;

private:

};

}
