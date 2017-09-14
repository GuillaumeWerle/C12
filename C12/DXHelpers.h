#pragma once

namespace DXHelpers
{
	const D3D12_HEAP_PROPERTIES* GetDefaultHeapProps();
	const D3D12_HEAP_PROPERTIES* GetUploadHeapProps();
	const D3D12_HEAP_PROPERTIES* GetReadbackHeapProps();

	inline u32 RGBA(u32 r, u32 g, u32 b, u32 a)
	{
		u32 c = r;
		c |= g << 8;
		c |= b << 16;
		c |= a << 24;
		return c;
	}
}

