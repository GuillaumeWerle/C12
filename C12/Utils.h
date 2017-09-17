#pragma once

template <u32 alignement>
u32 AlignOnPowerOfTwo(u32 val)
{
	return (val + alignement - 1) & ~(alignement - 1);
}

