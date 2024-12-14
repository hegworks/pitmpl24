#pragma once

namespace Uknitty
{
namespace Math
{

/// REF: Thanks to Hossein (245926) for coming up with the math
/// <summary>
/// map the number n from the range [x, y] to [a, b]
/// </summary>
inline float range_to_range(float x, float y, float a, float b, float n)
{
	return (((n - x) / (y - x)) * (b - a)) + a;
}

} // namespace Math
} // namespace Uknitty
