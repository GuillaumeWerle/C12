#pragma once

template <u32 alignement>
u32 AlignOnPowerOfTwo(u32 val)
{
	return (val + alignement - 1) & ~(alignement - 1);
}

template <typename T>
T Min(const T & a, const T & b)
{
	return (a < b) ? a : b;
}

template <typename T>
T Max(const T & a, const T & b)
{
	return (a > b) ? a : b;
}

