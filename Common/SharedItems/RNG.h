#pragma once

#include <cstdint>
#include <random>

namespace Uknitty
{

namespace RNG
{

using uint = uint32_t;

static uint32_t seed;

inline void RandomizeSeed()
{
	seed = std::random_device{}();
}

inline void SetSeed(uint32_t newSeed)
{
	seed = newSeed;
}

inline uint RandomUInt()
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;
	return seed;
}

inline float RandomFloat()
{
	return RandomUInt() * 2.3283064365387e-10f;
}

inline int RandomInt(int min, int max)
{
	return min + (RandomUInt() % (max - min + 1));
}

inline float RandomFloat(float min, float max)
{
	return min + (RandomFloat() * (max - min));
}

} // namespace RNG

} // namespace Uknitty
